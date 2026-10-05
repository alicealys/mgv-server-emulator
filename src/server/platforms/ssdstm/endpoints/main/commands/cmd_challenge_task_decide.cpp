#include <std_include.hpp>

#include "cmd_challenge_task_decide.hpp"

#include "database/models/challenge_tasks.hpp"

namespace emulator::ssd
{
	json::value cmd_challenge_task_decide::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		std::uint32_t id{};
		if (!json::read(id, data["id"]))
		{
			return error(ERR_INVALIDARG);
		}

		const auto expire_date = database::challenge_tasks::get_task_expire_date(id);
		if (!expire_date.has_value() || expire_date < std::chrono::system_clock::now().time_since_epoch())
		{
			return error(ERR_NOT_FOUND);
		}

		const auto order = database::challenge_tasks::find_order(user->current_player->get_player_id(), id);
		if (order.has_value())
		{
			return error(ERR_ALREADY_EXISTS);
		}

		const auto order_id = database::challenge_tasks::create_order(user->current_player->get_player_id(), id, 
			std::chrono::system_clock::time_point(expire_date.value()));
		if (order_id == 0ull)
		{
			return error(ERR_DATABASE);
		}

		return result;
	}

	std::uint32_t cmd_challenge_task_decide::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
