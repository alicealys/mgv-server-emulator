#include <std_include.hpp>

#include "cmd_matching_get_lock_set_room_owner.hpp"

#include "database/models/matching.hpp"

namespace emulator::ssd
{
	json::value cmd_matching_get_lock_set_room_owner::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		const auto room = database::matching::get_room_from_member(user->current_player->get_player_id());
		if (!room.has_value())
		{
			return error(ERR_NOT_IN_ROOM);
		}

		if (!database::matching::acquire_room_lock(room->get_room_id(), user->current_player->get_player_id()))
		{
			return error(ERR_ALREADY_LOCKED);
		}

		return result;
	}

	std::uint32_t cmd_matching_get_lock_set_room_owner::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
