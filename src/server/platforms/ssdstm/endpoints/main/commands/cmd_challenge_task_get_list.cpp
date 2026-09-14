#include <std_include.hpp>

#include "cmd_challenge_task_get_list.hpp"

namespace emulator::ssd
{
	json::value cmd_challenge_task_get_list::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		// TODO
		result["task_list"] = json::array{};
		result["expired_list"] = json::array{0, 0, 0, 0, 0};
		result["next_date"] = std::numeric_limits<std::int32_t>::max();

        return result;
	}
}
