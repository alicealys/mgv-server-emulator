#include <std_include.hpp>

#include "cmd_challenge_task_decide.hpp"

// not implemented
namespace emulator::ssd
{
	glz::json cmd_challenge_task_decide::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
