#include <std_include.hpp>

#include "cmd_matching_set_room_data_external.hpp"

#include "database/models/matching.hpp"

namespace emulator::ssd
{
	json::value cmd_matching_set_room_data_external::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		database::matching::set_data_external_param_t param{};
		if (!json::read(param, data["room_data"]))
		{
			return error(ERR_INVALIDARG);
		}

		const auto room = database::matching::get_room(param.room_id);
		if (!room.has_value())
		{
			return error(ERR_ROOM_NOT_FOUND);
		}

		if (room->get_owner_id() != user->current_player->get_player_id())
		{
			return error(ERR_PERMISSION_DENIED);
		}

		auto& int_attr = data["room_data"]["room_searchable_int_attr_external"];
		if (!int_attr.is_array())
		{
			std::memcpy(param.room_searchable_int_attr_external.data(), room->int_attr.data(), room->int_attr.size() * sizeof(std::int32_t));
		}

		database::matching::update_room(room->get_room_id(), param);

		return result;
	}

	std::uint32_t cmd_matching_set_room_data_external::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
