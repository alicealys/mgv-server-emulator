#include <std_include.hpp>

#include "cmd_mission_abort.hpp"

// not implemented
namespace emulator::ssd
{
	glz::json cmd_mission_abort::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
