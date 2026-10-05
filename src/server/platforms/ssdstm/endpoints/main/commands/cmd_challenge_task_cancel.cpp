#include <std_include.hpp>

#include "cmd_challenge_task_cancel.hpp"

#include "database/models/challenge_tasks.hpp"

namespace emulator::ssd
{
	json::value cmd_challenge_task_cancel::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		std::uint32_t id{};
		if (!json::read(id, data["id"]))
		{
			return error(ERR_INVALIDARG);
		}

		const auto order = database::challenge_tasks::find_order(user->current_player->get_player_id(), id);
		if (!order.has_value())
		{
			return error(ERR_NOT_FOUND);
		}

		if (order->complete() || !order->cancel())
		{
			return error(ERR_DATABASE);
		}

		return result;
	}

	std::uint32_t cmd_challenge_task_cancel::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
