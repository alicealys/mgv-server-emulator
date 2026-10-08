#include <std_include.hpp>

#include "cmd_matching_set_roommember_data.hpp"

#include "database/models/matching.hpp"

namespace emulator::ssd
{
	json::value cmd_matching_set_roommember_data::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		param_t param{};
		if (!json::read(param, data))
		{
			return error(ERR_INVALIDARG);
		}

		const auto member_data = utils::encoding::decode_url_string(param.roommember_data.room_member_bin_attr_internal.data);
		database::matching::set_member_data(user->current_player->get_player_id(), member_data);

		return result;
	}

	std::uint32_t cmd_matching_set_roommember_data::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
