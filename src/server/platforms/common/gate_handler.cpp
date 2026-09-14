#include <std_include.hpp>

#include "gate_handler.hpp"

#include "utils/encoding.hpp"
#include "game/game.hpp"

#include <utils/string.hpp>
#include <utils/compression.hpp>

namespace emulator
{
	gate_handler::gate_handler()
	{
		blow_.set_key(game::get_static_key(), game::get_static_key_len());
	}

	std::optional<glz::json> gate_handler::decrypt_request(const std::string& data, std::optional<database::users::user>&)
	{
		const auto decoded_data = utils::encoding::decode_url_string(data);
		const auto str = this->blow_.decrypt(decoded_data);
		if (str.empty())
		{
			return {};
		}

		glz::json json;
		if (glz::read_json(json, str))
		{
			return {};
		}
		
		const auto& compressed_val = json["compress"];
		if (!compressed_val.is_boolean())
		{
			return {};
		}

		const auto compressed = compressed_val.get<bool>();
		auto& json_data = json["data"];

		if (!json_data.is_string())
		{
			return {};
		}

		const auto& data_str = json_data.get<std::string>();
		std::string unescaped_data;

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

		if (glz::read_json(json_data, unescaped_data))
		{
			return {};
		}

		return std::make_optional(std::move(json));
	}

	bool gate_handler::verify_request(glz::json& request)
	{
		auto& data = request["data"];
		if (!data.is_object())
		{
			return false;
		}

		auto& session_crypto = request["session_crypto"];
		if (!session_crypto.is_boolean())
		{
			return false;
		}

		auto& session_key = request["session_key"];
		if (!session_key.is_string())
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

	std::optional<std::string> gate_handler::encrypt_response(glz::json& request, glz::json& data,
		const std::optional<database::users::user>&)
	{
		data["crypto_type"] = "COMMON";
		data["flowid"] = glz::json::object_t{};
		data["xuid"] = glz::json::object_t{};
		data["rqid"] = request["data"]["rqid"];
		data["msgid"] = request["data"]["msgid"];

		auto& result = data["result"];
		if (result.is_null())
		{
			result = "NOERR";
		}

		const auto data_dump_opt = data.dump();
		if (!data_dump_opt.has_value())
		{
			return {};
		}

		const auto& data_dump = data_dump_opt.value();
		const auto original_size = data_dump.size();

		auto data_res = utils::compression::zlib::compress(data_dump);
		data_res = utils::cryptography::base64::encode(data_res);

		glz::json response;

		response["compress"] = true;
		response["data"] = utils::encoding::split_into_lines(data_res);
		response["original_size"] = original_size;
		response["session_crypto"] = false;
		response["session_key"] = glz::json::object_t{};

		const auto response_str = response.dump();
		if (!response_str.has_value())
		{
			return {};
		}

		const auto encrypted = this->blow_.encrypt(response_str.value());
		auto encoded = utils::encoding::split_into_lines(encrypted);

		return std::make_optional(std::move(encoded));
	}
}
