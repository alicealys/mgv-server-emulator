#include <std_include.hpp>

#include "cmd_mission_start.hpp"

#include "database/models/players.hpp"

namespace emulator::ssd
{
	nlohmann::json cmd_mission_start::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;

		auto& current_mission_info_j = data["current_mission_info"];
		auto& story_sequence_number_j = data["story_sequence_number"];

		if (!current_mission_info_j.is_object() || !story_sequence_number_j.is_number_unsigned())
		{
			return error(ERR_INVALIDARG);
		}

		const auto mission_info = std::make_unique<database::players::mission_info_t>();
		mission_info->story_sequence_number = story_sequence_number_j;
		mission_info->parse(current_mission_info_j);

		user->current_player->set_mission_info(*mission_info);

        return result;
	}

	std::uint32_t cmd_mission_start::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
