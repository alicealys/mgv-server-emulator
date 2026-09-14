#include <std_include.hpp>

#include "cmd_challenge_task_complete.hpp"

// not implemented
namespace emulator::ssd
{
	glz::json cmd_challenge_task_complete::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
