#include <std_include.hpp>

#include "cmd_get_daily_reward.hpp"

// not implemented
namespace emulator::ssd
{
	nlohmann::json cmd_get_daily_reward::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
