#include <std_include.hpp>

#include "cmd_challenge_task_get_list.hpp"

#include "database/models/challenge_tasks.hpp"

namespace emulator::ssd
{
	json::value cmd_challenge_task_get_list::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		result["next_date"] = database::challenge_tasks::get_next_date().count();
		database::challenge_tasks::dump_order_list(user->current_player->get_player_id(), 
			result["expired_list"], result["task_list"]);

		return result;
	}

	std::uint32_t cmd_challenge_task_get_list::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
