#include <std_include.hpp>

#include "cmd_challenge_task_progress.hpp"

#include "database/models/challenge_tasks.hpp"

namespace emulator::ssd
{
	json::value cmd_challenge_task_progress::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		database::challenge_tasks::send_progress_list(user->current_player->get_player_id(), data["order_progress_list"]);

		return result;
	}

	std::uint32_t cmd_challenge_task_progress::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
