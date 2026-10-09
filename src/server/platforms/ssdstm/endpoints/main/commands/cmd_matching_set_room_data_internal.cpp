#include <std_include.hpp>

#include "cmd_matching_set_room_data_internal.hpp"

#include "database/models/matching.hpp"

namespace emulator::ssd
{
	json::value cmd_matching_set_room_data_internal::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		database::matching::set_data_internal_param_t param{};
		if (!json::read(param, data["room_data"]))
		{
			return error(ERR_INVALIDARG);
		}

		const auto room = database::matching::get_room(param.room_id);
		if (!room.has_value())
		{
			return result;
		}

		if (room->get_owner_id() != user->current_player->get_player_id())
		{
			return result;
		}

		database::matching::update_room(room->get_room_id(), param);

		return result;
	}

	std::uint32_t cmd_matching_set_room_data_internal::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
