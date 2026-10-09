#include <std_include.hpp>

#include "cmd_matching_set_room_data_external.hpp"

#include "database/models/matching.hpp"

namespace emulator::ssd
{
	json::value cmd_matching_set_room_data_external::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		param_t param{};
		if (!json::read(param, data))
		{
			return error(ERR_INVALIDARG);
		}

		const auto room = database::matching::get_room(param.room_data.room_id);
		if (!room.has_value())
		{
			return result; //error(ERR_ROOM_NOT_FOUND);
		}

		if (room->get_owner_id() != user->current_player->get_player_id())
		{
			return result; // error(ERR_PERMISSION_DENIED);
		}

		database::matching::set_data_external_param_t update_param{};
		std::memcpy(update_param.room_searchable_int_attr_external.data(), room->int_attr.data(), room->int_attr.size() * sizeof(std::int64_t));
		update_param.room_searchable_bin_attr_external[0].data = room->bin_attr[0];
		update_param.room_searchable_bin_attr_external[1].data = room->bin_attr[1];

		for (const auto& entry : param.room_data.room_searchable_int_attr_external)
		{
			if (entry.index < update_param.room_searchable_int_attr_external.size())
			{
				update_param.room_searchable_int_attr_external[entry.index] = entry.value;
			}
		}

		for (const auto& entry : param.room_data.room_searchable_bin_attr_external)
		{
			if (entry.index < update_param.room_searchable_bin_attr_external.size())
			{
				update_param.room_searchable_bin_attr_external[entry.index].data = utils::encoding::decode_url_string(entry.data);
			}
		}

		database::matching::update_room(room->get_room_id(), update_param);

		return result;
	}

	std::uint32_t cmd_matching_set_room_data_external::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
