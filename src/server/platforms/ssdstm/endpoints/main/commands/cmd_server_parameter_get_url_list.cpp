#include <std_include.hpp>

#include "cmd_server_parameter_get_url_list.hpp"
#include "game/parameters.hpp"

#include <utils/cryptography.hpp>

namespace emulator::ssd
{
	nlohmann::json cmd_server_parameter_get_url_list::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;

		static const auto base_url = config::get<std::string>("base_url");
		for (auto i = 0ull; i < game::parameters_list.size(); i++)
		{
			const auto& parameter = game::parameters_list[i];
			auto& entry = result["server_parameter_file_info_list"][i];
			entry["url"] = std::format("{}/ssdstmweb/parameter/{}.param", base_url, parameter->name());
			entry["md5"] = parameter->get_data_md5();
		}

        return result;
	}
}
