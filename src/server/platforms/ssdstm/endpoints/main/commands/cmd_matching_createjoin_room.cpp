#include <std_include.hpp>

#include "cmd_matching_createjoin_room.hpp"

#include "database/models/matching.hpp"

namespace emulator::ssd
{
	json::value cmd_matching_createjoin_room::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		database::matching::create_param_t param{};
		if (!json::read(param, data["createjoin_param"]))
		{
			return error(ERR_INVALIDARG);
		}

		if (param.max_slot > database::matching::max_room_members)
		{
			return error(ERR_INVALIDARG);
		}

		database::matching::remove_member(user->current_player->get_player_id());
		database::matching::close_room_from_owner(user->current_player->get_player_id());

		param.room_searchable_bin_attr_external[0].data = utils::encoding::decode_url_string(param.room_searchable_bin_attr_external[0].data);
		param.room_searchable_bin_attr_external[1].data = utils::encoding::decode_url_string(param.room_searchable_bin_attr_external[1].data);

		const auto room_id = database::matching::create_room(user->current_player->get_player_id(), param);
		if (room_id == 0ull)
		{
			return error(ERR_DATABASE);
		}

		const auto member_data = utils::encoding::decode_url_string(param.room_member_bin_attr_internal.data);
		database::matching::add_member(room_id, user->current_player->get_player_id(), member_data);
		result["room_id"] = room_id;

		return result;
	}

	std::uint32_t cmd_matching_createjoin_room::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
