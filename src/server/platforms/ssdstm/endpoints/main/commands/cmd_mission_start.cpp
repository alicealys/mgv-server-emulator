#include <std_include.hpp>

#include "cmd_mission_start.hpp"

#include "database/models/players.hpp"

namespace emulator::ssd
{
	json::value cmd_mission_start::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		auto& current_mission_info_j = data["current_mission_info"];
		auto& story_sequence_number_j = data["story_sequence_number"];

		if (!current_mission_info_j.is_object() || !story_sequence_number_j.is_uint64())
		{
			return error(ERR_INVALIDARG);
		}

		const auto mission_info = std::make_unique<database::players::mission_info_t>();
		const auto story_unlock_info = std::make_unique<database::players::story_unlock_info_t>();
		user->current_player->get_story_unlock_info(*story_unlock_info);

		story_unlock_info->story_sequence_number = story_sequence_number_j.as<std::uint32_t>();
		mission_info->parse(current_mission_info_j);

		user->current_player->set_mission_info(*mission_info);
		user->current_player->set_story_unlock_info(*story_unlock_info);

        return result;
	}

	std::uint32_t cmd_mission_start::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
