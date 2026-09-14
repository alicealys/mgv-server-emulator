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

#ifdef DEBUG
		const auto start = std::chrono::high_resolution_clock::now();
		const auto _0 = gsl::finally([=]
		{
			const auto end = std::chrono::high_resolution_clock::now();
			const auto diff = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
			console::debug("[Endpoint] decrypt request took %lli us", diff);
		});
#endif
		const auto decoded_data = utils::encoding::decode_url_string(data);
		const auto str = this->blow_.decrypt(decoded_data);
		if (str.empty())
		{
			return json;
		}

		const auto error = json::read(json.emplace(), str);
		if (error)
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
			user = database::users::find_by_session_id(session_key, false);
			if (!user.has_value())
			{
				data_j = json::null{};
				return {};
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

		if (json::read(data_j, unescaped_data))
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
#ifdef DEBUG
		const auto start = std::chrono::high_resolution_clock::now();
		const auto _0 = gsl::finally([=]
		{
			const auto end = std::chrono::high_resolution_clock::now();
			const auto diff = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
			console::debug("[Endpoint] encrypt response took %lli us", diff);
		});
#endif

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
		data["rqid"] = request["data"]["rqid"];
		data["msgid"] = request["data"]["msgid"];

		if (data["result"].is_null())
		{
			data["result"] = "NOERR";
		}

		const auto data_dump = json::dump(data);
		const auto original_size = data_dump.size();

		auto data_res = utils::compression::zlib::compress(data_dump, 1u);
		data_res.push_back('\0');

		if (!session_crypto)
		{
			data_res = utils::cryptography::base64::encode(data_res);
		}
		else
		{
			if (!user.has_value())
			{
				return false;
			}

			utils::cryptography::blowfish session_blow;
			session_blow.set_key(user->get_crypto_key());
			data_res = session_blow.encrypt(data_res);
		}

		json::value response;

		response["compress"] = true;
		response["data"] = utils::encoding::split_into_lines(data_res);
		response["original_size"] = original_size;
		response["session_crypto"] = session_crypto_val;
		response["session_key"] = request["session_key"];

		const auto response_str = json::dump(response);
		const auto encrypted = this->blow_.encrypt(response_str);
		result = utils::encoding::split_into_lines(encrypted);
		
		return true;
	}
}
