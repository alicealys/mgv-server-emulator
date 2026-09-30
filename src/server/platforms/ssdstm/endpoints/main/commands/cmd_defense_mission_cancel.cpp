#include <std_include.hpp>

#include "cmd_defense_mission_cancel.hpp"
#include "database/models/defense_missions.hpp"

namespace emulator::ssd
{
	json::value cmd_defense_mission_cancel::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		const auto mission = database::defense_missions::get_current_mission(user->current_player->get_player_id());
		if (!mission.has_value())
		{
			return error(ERR_DEFENSE_MISSION_NOT_IN_MISSION);
		}

		mission->update(2, mission->get_current_wave(), mission->get_clear_rank(),
			mission->get_total_score(), mission->get_end_date(), mission->get_next_wave_date());

		return result;
	}

	std::uint32_t cmd_defense_mission_cancel::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
