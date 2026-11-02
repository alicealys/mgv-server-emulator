#include <std_include.hpp>

#include "main_handler.hpp"

#include "database/database.hpp"
#include "database/models/players.hpp"

#include "utils/encoding.hpp"
#include "game/game.hpp"

#include <utils/string.hpp>
#include <utils/compression.hpp>

namespace emulator
{
	main_handler::main_handler()
	{
		blow_.set_key(game::get_static_key(), game::get_static_key_len());
	}

	std::optional<json::value> main_handler::decrypt_request(const std::string& data, std::optional<database::users::user>& user)
	{
		std::optional<json::value> json;

		const auto str = this->blow_.decrypt(data);
		if (str.empty())
		{
			return json;
		}

		if (!json::read(json.emplace(), str))
		{
			return {};
		}

		auto& data_j = json->operator[]("data");
		if (!data_j.is_string())
		{
			return {};
		}

		auto& compress_j = json->operator[]("compress");
		if (!compress_j.is_boolean())
		{
			return {};
		}

		const auto compress = compress_j.get<bool>();
		auto& session_crypto_j = json->operator[]("session_crypto");

		std::string data_str = data_j.get<std::string>();
		std::string unescaped_data;

		if (session_crypto_j.is_boolean() && session_crypto_j.as<bool>())
		{
			auto& session_key_j = json->operator[]("session_key");
			if (!session_key_j.is_string())
			{
				return {};
			}

			const auto& session_key = session_key_j.get<std::string>();
			const auto found_user = database::users::find_from_session_id(session_key, true);
			if (!found_user.has_value())
			{
				data_j = json::null{};
				return {};
			}

			user.emplace(found_user.value());

			if (found_user->current_player.has_value())
			{
				user->current_player.emplace(found_user->current_player.value());
			}

			utils::cryptography::blowfish session_blow;
			session_blow.set_key(user->get_crypto_key());

			const auto decrypted = session_blow.decrypt(data_str);
			if (!compress)
			{
				unescaped_data = utils::encoding::unescape_json(decrypted);
			}
			else
			{
				const auto decompressed = utils::compression::zlib::decompress(decrypted);
				unescaped_data = utils::encoding::unescape_json(decompressed);
			}
		}
		else
		{
			if (!compress)
			{
				unescaped_data = utils::encoding::unescape_json(data_str);
			}
			else
			{
				const auto decoded = utils::cryptography::base64::decode(data_str);
				const auto decompressed = utils::compression::zlib::decompress(decoded);
				unescaped_data = utils::encoding::unescape_json(decompressed);
			}
		}

		if (!json::read(data_j, unescaped_data))
		{
			return {};
		}

		return json;
	}

	bool main_handler::verify_request(json::value& request)
	{
		auto& data = request["data"];
		if (!data.is_object())
		{
			return false;
		}

		auto& session_crypto = request["session_crypto"];
		auto& session_key = request["session_key"];
		if (!session_crypto.is_boolean())
		{
			return false;
		}

		if (session_crypto.get<bool>() && !session_key.is_string())
		{
			return false;
		}

		auto& msgid = data["msgid"];
		if (!msgid.is_string())
		{
			return false;
		}

		return true;
	}

	bool main_handler::encrypt_response(json::value& request, json::value& data,
		const std::optional<database::users::user>& user, std::string& result)
	{
		const auto& session_crypto_val = request["session_crypto"];
		const auto session_crypto = session_crypto_val.is_boolean() && session_crypto_val.get<bool>();

		if (session_crypto)
		{
			data["crypto_type"] = "COMPOUND";
		}
		else
		{
			data["crypto_type"] = "COMMON";
		}

		data["flowid"] = json::null{};
		data["xuid"] = json::null{};

		auto& request_data_j = request["data"];
		data["rqid"] = request_data_j["rqid"];
		data["msgid"] = request_data_j["msgid"];

		auto& result_j = data["result"];
		if (result_j.is_null())
		{
			result_j = "NOERR";
		}

		auto data_dump = json::dump(data);
		const auto original_size = data_dump.size();

		data_dump = utils::compression::zlib::compress(data_dump, 1u, Z_BEST_SPEED);
		data_dump.push_back('\0');

		if (!session_crypto)
		{
			data_dump = utils::cryptography::base64::encode(data_dump);
		}
		else
		{
			if (!user.has_value())
			{
				return false;
			}

			utils::cryptography::blowfish session_blow;
			session_blow.set_key(user->get_crypto_key());
			data_dump = session_blow.encrypt(data_dump);
		}

		json::value response;

		response["compress"] = true;
		response["data"] = utils::encoding::split_into_lines(data_dump);
		response["original_size"] = original_size;
		response["session_crypto"] = session_crypto_val;
		response["session_key"] = request["session_key"];

		const auto response_str = json::dump(response);
		const auto encrypted = this->blow_.encrypt(response_str);
		result = utils::encoding::split_into_lines(encrypted);
		
		return true;
	}
}
