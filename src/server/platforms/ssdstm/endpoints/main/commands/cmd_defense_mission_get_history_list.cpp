#include <std_include.hpp>

#include "cmd_defense_mission_get_history_list.hpp"

// not implemented
namespace emulator::ssd
{
	nlohmann::json cmd_defense_mission_get_history_list::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
