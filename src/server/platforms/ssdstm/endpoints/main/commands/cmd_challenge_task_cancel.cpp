#include <std_include.hpp>

#include "cmd_challenge_task_cancel.hpp"

// not implemented
namespace emulator::ssd
{
	glz::json cmd_challenge_task_cancel::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
