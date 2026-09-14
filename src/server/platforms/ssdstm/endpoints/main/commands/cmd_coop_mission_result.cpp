#include <std_include.hpp>

#include "cmd_coop_mission_result.hpp"

// not implemented
namespace emulator::ssd
{
	json::value cmd_coop_mission_result::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
