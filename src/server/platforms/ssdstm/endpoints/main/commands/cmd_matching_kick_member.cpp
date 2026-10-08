#include <std_include.hpp>

#include "cmd_matching_kick_member.hpp"

#include "database/models/matching.hpp"

namespace emulator::ssd
{
	json::value cmd_matching_kick_member::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		param_t param{};
		if (!json::read(param, data))
		{
			return error(ERR_INVALIDARG);
		}

		const auto target_user = database::users::find_from_account(param.first_party_id);
		if (!target_user.has_value())
		{
			return error(ERR_PLAYER_NOTFOUND);
		}

		const auto room = database::matching::get_room_from_member(user->current_player->get_player_id());
		if (!room.has_value() || room->get_room_id() != param.room_id)
		{
			return error(ERR_ROOM_NOT_FOUND);
		}

		if (room->get_owner_id() != user->current_player->get_player_id())
		{
			return error(ERR_PERMISSION_DENIED);
		}

		database::matching::remove_member(room->get_room_id(), target_user->current_player->get_player_id());

		return result;
	}

	std::uint32_t cmd_matching_kick_member::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
