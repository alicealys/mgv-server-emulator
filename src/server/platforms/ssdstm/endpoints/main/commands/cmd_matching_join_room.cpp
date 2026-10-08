#include <std_include.hpp>

#include "cmd_matching_join_room.hpp"

#include "database/models/matching.hpp"

namespace emulator::ssd
{
	json::value cmd_matching_join_room::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		param_t param{};
		if (!json::read(param, data))
		{
			return error(ERR_INVALIDARG);
		}

		const auto room = database::matching::get_room(param.room_id);
		if (!room.has_value())
		{
			return error(ERR_ROOM_NOT_FOUND);
		}

		const auto members = database::matching::get_members(room->get_room_id());
		if (members.size() >= room->get_max_slot())
		{
			return error(ERR_ROOMTOOMANY);
		}

		if (param.password != room->get_password())
		{
			return error(ERR_PASSWORD_DO_NOT_MATCH);
		}

		const auto current_room = database::matching::get_room_from_member(user->current_player->get_player_id());
		if (current_room.has_value() && current_room->get_room_id() == room->get_room_id())
		{
			return error(ERR_ALREADYINROOM);
		}

		if (current_room.has_value() && current_room->get_owner_id() == user->current_player->get_player_id())
		{
			database::matching::migrate_room_owner(current_room->get_room_id(), current_room->get_owner_id());
		}

		database::matching::remove_member(user->current_player->get_player_id());

		if (current_room.has_value())
		{
			database::matching::check_close_room(current_room->get_room_id());
		}

		const auto member_id = database::matching::add_member(room->get_room_id(), user->current_player->get_player_id());
		if (member_id == 0ull)
		{
			return error(ERR_DATABASE);
		}

		return result;
	}

	std::uint32_t cmd_matching_join_room::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
