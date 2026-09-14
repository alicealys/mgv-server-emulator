#include <std_include.hpp>

#include "cmd_defense_mission_start.hpp"

// not implemented
namespace emulator::ssd
{
	glz::json cmd_defense_mission_start::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
