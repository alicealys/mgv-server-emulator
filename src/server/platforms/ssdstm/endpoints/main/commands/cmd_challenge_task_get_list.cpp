#include <std_include.hpp>

#include "cmd_challenge_task_get_list.hpp"

namespace emulator::ssd
{
	glz::json cmd_challenge_task_get_list::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;

		// TODO
		result["task_list"] = glz::json::array_t{};
		result["expired_list"] = glz::json::array_t{0, 0, 0, 0, 0};
		result["next_date"] = std::numeric_limits<std::int32_t>::max();

        return result;
	}
}
