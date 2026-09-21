#include <std_include.hpp>

#include "cmd_defense_mission_get_history_list.hpp"
#include "database/models/defense_missions.hpp"

namespace emulator::ssd
{
	json::value cmd_defense_mission_get_history_list::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		const auto history = database::defense_missions::get_history(user->current_player->get_player_id());
		for (auto i = 0ull; i < history.size(); i++)
		{
			history[i].to_json(result["defense_mission_history_list"][i]);
		}

        return result;
	}

	std::uint32_t cmd_defense_mission_get_history_list::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
