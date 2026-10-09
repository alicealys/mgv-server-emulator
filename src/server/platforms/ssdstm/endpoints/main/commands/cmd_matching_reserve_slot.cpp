#include <std_include.hpp>

#include "cmd_matching_reserve_slot.hpp"

#include "database/models/matching.hpp"

namespace emulator::ssd
{
	json::value cmd_matching_reserve_slot::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		param_t param{};
		if (!json::read(param, data))
		{
			return error(ERR_INVALIDARG);
		}

		const auto room = database::matching::get_room_from_member(user->current_player->get_player_id());
		if (!room.has_value() || room->get_room_id() != param.room_id)
		{
			return error(ERR_ROOM_NOT_FOUND);
		}

		const auto members = database::matching::get_members(room->get_room_id());
		if (members.size() + param.reserve_num > room->get_max_slot())
		{
			return error(ERR_ROOMTOOMANY);
		}

		database::matching::set_room_reserve_num(room->get_room_id(), param.reserve_num);

		return result;
	}

	std::uint32_t cmd_matching_reserve_slot::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
