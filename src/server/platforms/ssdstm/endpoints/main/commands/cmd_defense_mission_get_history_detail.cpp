#include <std_include.hpp>

#include "cmd_defense_mission_get_history_detail.hpp"
#include "database/models/defense_missions.hpp"

namespace emulator::ssd
{
	json::value cmd_defense_mission_get_history_detail::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		std::uint32_t history_unique_id{};
		if (json::read(history_unique_id, data["history_unique_id"]))
		{
			return error(ERR_INVALIDARG);
		}

		const auto mission = database::defense_missions::find(history_unique_id);
		if (!mission.has_value())
		{
			return error(ERR_DATABASE);
		}

		const auto waves = mission->get_waves();
		for (auto i = 0ull; i < waves.size(); i++)
		{
			waves[i].to_json(result["defense_mission_wave_history"][i]);
		}

        return result;
	}

	std::uint32_t cmd_defense_mission_get_history_detail::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
