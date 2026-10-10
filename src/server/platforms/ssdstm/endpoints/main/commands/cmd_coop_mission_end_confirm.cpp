#include <std_include.hpp>

#include "cmd_coop_mission_end_confirm.hpp"

#include "database/models/matching.hpp"

namespace emulator::ssd
{
	json::value cmd_coop_mission_end_confirm::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		const auto current_room = database::matching::get_room_from_member(user->current_player->get_player_id());
		if (current_room.has_value() && current_room->get_owner_player_id() == user->current_player->get_player_id())
		{
			database::matching::set_room_status(current_room->get_room_id(), database::matching::status_mission_end_confirm);
		}

		return result;
	}

	std::uint32_t cmd_coop_mission_end_confirm::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
