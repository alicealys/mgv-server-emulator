#include <std_include.hpp>

#include "cmd_coop_lobby_end.hpp"
#include "cmd_inventory_save.hpp"

#include "database/models/matching.hpp"

namespace emulator::ssd
{
	json::value cmd_coop_lobby_end::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		cmd_inventory_save::do_save(data, user);

		auto& tips_open_info_j = data["tips_open_info"];
		auto& avatar_condition_j = data["avatar_condition"];

		if (tips_open_info_j.is_array() && tips_open_info_j.size())
		{
			const auto story_unlock_info = std::make_unique<database::players::story_unlock_info_t>();
			user->current_player->get_story_unlock_info(*story_unlock_info);
			story_unlock_info->tips_open_info.parse(tips_open_info_j[0]);
			user->current_player->set_story_unlock_info(*story_unlock_info);
		}

		if (avatar_condition_j.is_array() && avatar_condition_j.size())
		{
			const auto mission_info = std::make_unique<database::players::mission_info_t>();
			user->current_player->get_mission_info(*mission_info);
			mission_info->parse_avatar_condition(avatar_condition_j[0]);
			user->current_player->set_mission_info(*mission_info);
		}

		return result;
	}

	std::uint32_t cmd_coop_lobby_end::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
