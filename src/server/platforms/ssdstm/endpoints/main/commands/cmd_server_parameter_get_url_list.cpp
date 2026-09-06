#include <std_include.hpp>

#include "cmd_server_parameter_get_url_list.hpp"

// not implemented
namespace emulator::ssd
{
	nlohmann::json cmd_server_parameter_get_url_list::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
