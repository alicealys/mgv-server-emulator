#include <std_include.hpp>

#include "cmd_server_parameter_get_url_list.hpp"
#include "game/parameters.hpp"

#include <utils/cryptography.hpp>

namespace emulator::ssd
{
	json::value cmd_server_parameter_get_url_list::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		static const auto base_url = config::get().base_url;
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
