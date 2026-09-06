#include <std_include.hpp>

#include "cmd_challenge_task_decide.hpp"

// not implemented
namespace emulator::ssd
{
	nlohmann::json cmd_challenge_task_decide::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
