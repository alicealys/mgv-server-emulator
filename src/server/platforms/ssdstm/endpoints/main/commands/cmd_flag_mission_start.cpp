#include <std_include.hpp>

#include "cmd_flag_mission_start.hpp"

// not implemented
namespace emulator::ssd
{
	nlohmann::json cmd_flag_mission_start::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
