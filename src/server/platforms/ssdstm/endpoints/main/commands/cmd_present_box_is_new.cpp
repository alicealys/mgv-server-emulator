#include <std_include.hpp>

#include "cmd_present_box_is_new.hpp"

#include "database/models/present_box.hpp"

namespace emulator::ssd
{
	json::value cmd_present_box_is_new::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		const auto has_new = database::present_box::has_new_item(user->current_player->get_player_id());
		result["is_new"] = std::uint8_t(has_new);

        return result;
	}

	std::uint32_t cmd_present_box_is_new::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
