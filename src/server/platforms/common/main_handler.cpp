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
			return {};
		}

		json::value json;
		const auto error = json::read(json, str);
		if (error)
		{
			return {};
		}

		if (!json["data"].is_string())
		{
			return {json};
		}

		const auto& compressed_val = json["compress"];
		if (!compressed_val.is_boolean())
		{
			return {json};
		}

		const auto compressed = compressed_val.get<bool>();
		const auto& session_crypto = json["session_crypto"];
		auto& json_data = json["data"];

		std::string data_str = json_data.get<std::string>();
		std::string unescaped_data;

		if (session_crypto.is_boolean() && session_crypto.get<bool>())
		{
			const auto& session_key = json["session_key"].get<std::string>();
			user = database::users::find_by_session_id(session_key, false);
			if (!user.has_value())
			{
				json_data = json::value::object_t{};
				return {json};
			}

			utils::cryptography::blowfish session_blow;
			session_blow.set_key(user->get_crypto_key());

			const auto decrypted = session_blow.decrypt(data_str);
			if (!compressed)
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
			if (!compressed)
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

		if (json::read(json_data, unescaped_data))
		{
			return {};
		}

		return std::make_optional(std::move(json));
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

	std::optional<std::string> main_handler::encrypt_response(json::value& request, json::value& data,
		const std::optional<database::users::user>& user)
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

		data["flowid"] = json::value::object_t{};
		data["xuid"] = json::value::object_t{};
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
				return {};
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
		auto encoded = utils::encoding::split_into_lines(encrypted);
		
		return std::make_optional(std::move(encoded));
	}
}
