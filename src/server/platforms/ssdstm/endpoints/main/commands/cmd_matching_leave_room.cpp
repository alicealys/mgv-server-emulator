#include <std_include.hpp>

#include "cmd_matching_leave_room.hpp"

#include "database/models/matching.hpp"

namespace emulator::ssd
{
	json::value cmd_matching_leave_room::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		database::matching::remove_member(user->current_player->get_player_id());

		return result;
	}

	std::uint32_t cmd_matching_leave_room::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
