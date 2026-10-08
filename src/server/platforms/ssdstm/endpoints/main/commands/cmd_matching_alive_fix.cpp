#include <std_include.hpp>

#include "cmd_matching_alive_fix.hpp"

#include "database/models/matching.hpp"

namespace emulator::ssd
{
	json::value cmd_matching_alive_fix::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		[[ maybe_unused ]] const auto room = database::matching::get_room_from_member(user->current_player->get_player_id());

		return result;
	}

	std::uint32_t cmd_matching_alive_fix::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
