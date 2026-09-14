#include <std_include.hpp>

#include "cmd_defense_mission_get_history_detail.hpp"

// not implemented
namespace emulator::ssd
{
	json::value cmd_defense_mission_get_history_detail::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
