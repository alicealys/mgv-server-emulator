#include <std_include.hpp>

#include "cmd_server_parameter_load.hpp"

namespace emulator::ssd
{
	cmd_server_parameter_load::cmd_server_parameter_load()
	{
		this->list_ = resource(RESOURCE_SERVER_PARAMETER_LIST);
	}

	json::value cmd_server_parameter_load::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

        result["server_parameter_list"] = this->list_;

        return result;
	}
}
