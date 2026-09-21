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

	std::optional<json::value> gate_handler::decrypt_request(const std::string& data, std::optional<database::users::user>&)
	{
		const auto decoded_data = utils::encoding::decode_url_string(data);
		const auto str = this->blow_.decrypt(decoded_data);
		if (str.empty())
		{
			return {};
		}

		std::optional<json::value> json;
		if (!json::read(json.emplace(), str))
		{
			return {};
		}
		
		auto& compress_j = json->operator[]("compress");
		if (!compress_j.is_boolean())
		{
			return {};
		}

		auto& data_j = json->operator[]("data");
		if (!data_j.is_string())
		{
			return {};
		}

		const auto& data_str = data_j.get<std::string>();
		std::string unescaped_data;

		const auto compress = compress_j.get<bool>();
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

		if (!json::read(data_j, unescaped_data))
		{
			return {};
		}

		return json;
	}

	bool gate_handler::verify_request(json::value& request)
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

	bool gate_handler::encrypt_response(json::value& request, json::value& data,
		const std::optional<database::users::user>&, std::string& result)
	{
		data["crypto_type"] = "COMMON";
		data["flowid"] = json::null{};
		data["xuid"] = json::null{};
		data["rqid"] = request["data"]["rqid"];
		data["msgid"] = request["data"]["msgid"];

		auto& result_j = data["result"];
		if (result_j.is_null())
		{
			result_j = "NOERR";
		}

		const auto data_dump = json::dump(data);
		const auto original_size = data_dump.size();

		auto data_res = utils::compression::zlib::compress(data_dump);
		data_res = utils::cryptography::base64::encode(data_res);

		json::value response;

		response["compress"] = true;
		response["data"] = utils::encoding::split_into_lines(data_res);
		response["original_size"] = original_size;
		response["session_crypto"] = false;
		response["session_key"] = json::null{};

		const auto response_str = json::dump(response);
		const auto encrypted = this->blow_.encrypt(response_str);
		result = utils::encoding::split_into_lines(encrypted);

		return true;
	}
}
