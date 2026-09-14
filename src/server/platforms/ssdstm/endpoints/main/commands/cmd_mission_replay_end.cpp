#include <std_include.hpp>

#include "cmd_mission_replay_end.hpp"

// not implemented
namespace emulator::ssd
{
	glz::json cmd_mission_replay_end::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
