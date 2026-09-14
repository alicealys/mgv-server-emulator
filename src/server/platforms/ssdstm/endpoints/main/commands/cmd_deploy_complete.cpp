#include <std_include.hpp>

#include "cmd_deploy_complete.hpp"

// not implemented
namespace emulator::ssd
{
	json::value cmd_deploy_complete::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
