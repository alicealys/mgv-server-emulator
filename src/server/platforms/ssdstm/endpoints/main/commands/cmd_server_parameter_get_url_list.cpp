#include <std_include.hpp>

#include "cmd_server_parameter_get_url_list.hpp"

#include <utils/cryptography.hpp>

namespace emulator::ssd
{
	cmd_server_parameter_get_url_list::cmd_server_parameter_get_url_list()
	{
		const auto base_url = config::get<std::string>("base_url");
		auto list = resource(RESOURCE_SERVER_PARAMETER_LIST);

		for (auto i = 0ull; i < list["file_list"].size(); i++)
		{
			const auto name = list["file_list"][i].get<std::string>();
			const auto path = std::format("resources/data/parameter/{}", name);
			const auto resource_id = utils::resources::get_resource_id(path);
			if (!resource_id.has_value())
			{
				continue;
			}

			const auto data = utils::resources::load(resource_id.value());
			const auto md5 = utils::cryptography::md5::compute(data, true);

			this->list_[i]["md5"] = utils::string::to_lower(md5);
			this->list_[i]["url"] = std::format("{}/ssdstmweb/parameter/{}", base_url, name);
		}
	}

	nlohmann::json cmd_server_parameter_get_url_list::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;

		result["server_parameter_file_info_list"] = this->list_;

        return result;
	}
}
