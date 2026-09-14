#include <std_include.hpp>

#include "cmd_mission_abort.hpp"

// not implemented
namespace emulator::ssd
{
	json::value cmd_mission_abort::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
