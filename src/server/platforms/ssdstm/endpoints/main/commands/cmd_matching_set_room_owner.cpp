#include <std_include.hpp>

#include "cmd_matching_set_room_owner.hpp"

#include "database/models/matching.hpp"

namespace emulator::ssd
{
	json::value cmd_matching_set_room_owner::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		param_t param{};
		if (!json::read(param, data))
		{
			return error(ERR_INVALIDARG);
		}

		const auto room = database::matching::get_room_from_member(param.new_owner_player_id);
		if (!room.has_value() || room->get_room_id() != param.room_id)
		{
			return error(ERR_ROOM_NOT_FOUND);
		}

		if (room->get_owner_id() != user->current_player->get_player_id())
		{
			return error(ERR_PERMISSION_DENIED);
		}

		database::matching::set_room_owner(room->get_room_id(), param.new_owner_player_id);

		return result;
	}

	std::uint32_t cmd_matching_set_room_owner::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
