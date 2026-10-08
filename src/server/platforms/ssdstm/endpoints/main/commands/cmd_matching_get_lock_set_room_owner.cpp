#include <std_include.hpp>

#include "cmd_matching_get_lock_set_room_owner.hpp"

#include "database/models/matching.hpp"

namespace emulator::ssd
{
	json::value cmd_matching_get_lock_set_room_owner::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		const auto room = database::matching::get_room_from_member(user->current_player->get_player_id());
		if (room->get_owner_id() == user->current_player->get_player_id())
		{
			return result;
		}

		//database::matching::set_room_owner(room->get_room_id(), user->current_player->get_player_id());

		return result;
	}

	std::uint32_t cmd_matching_get_lock_set_room_owner::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
