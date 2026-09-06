#include <std_include.hpp>

#include "cmd_challenge_task_get_list.hpp"

namespace emulator::ssd
{
	nlohmann::json cmd_challenge_task_get_list::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;

		// TODO
		result["task_list"] = nlohmann::json::array();
		result["expired_list"] = {0, 0, 0, 0, 0};
		result["next_date"] = std::numeric_limits<std::int32_t>::max();

        return result;
	}
}
