#include <std_include.hpp>

#include "cmd_challenge_task_cancel.hpp"

// not implemented
namespace emulator::ssd
{
	json::value cmd_challenge_task_cancel::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
