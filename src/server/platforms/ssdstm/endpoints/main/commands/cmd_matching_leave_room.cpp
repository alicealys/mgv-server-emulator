#include <std_include.hpp>

#include "cmd_matching_leave_room.hpp"

#include "database/models/matching.hpp"

namespace emulator::ssd
{
	json::value cmd_matching_leave_room::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		const auto room = database::matching::get_room_from_member(user->current_player->get_player_id());
		if (!room.has_value())
		{
			return result;
		}

		if (room->get_owner_player_id() == user->current_player->get_player_id())
		{
			database::matching::migrate_room_owner(room->get_room_id(), room->get_owner_player_id());
		}

		database::matching::remove_member(user->current_player->get_player_id());
		database::matching::check_close_room(room->get_room_id());

		return result;
	}

	std::uint32_t cmd_matching_leave_room::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
