#include <std_include.hpp>

#include "cmd_defense_mission_start.hpp"

#include "database/models/defense_missions.hpp"

namespace emulator::ssd
{
	json::value cmd_defense_mission_start::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		param_t param{};
		if (!json::read(param, data))
		{
			return error(ERR_INVALIDARG);
		}

		const auto iter = game::parameters_table.ssd_base_defense_settings->mission_settings.find(param.mission_code);
		if (iter == game::parameters_table.ssd_base_defense_settings->mission_settings.end())
		{
			return error(ERR_INVALIDARG);
		}

		database::players::defense_mission_info_t info{};
		user->current_player->get_defense_mission_info(info);

		if (info.status.mining_machine_life == 0)
		{
			return error(ERR_DATABASE);
		}

		if (param.wave != 0 || param.mission_code == 0)
		{
			return result;
		}

		const auto mission = database::defense_missions::get_current_mission(user->current_player->get_player_id());
		if (mission.has_value() && mission->get_mission_code() == param.mission_code)
		{
			return result;
		}

		database::defense_missions::create_mission(user->current_player->get_player_id(), param.mission_code);

		info.parameter.threat = 0;
		info.parameter.threat_threshold = 0;
		info.parameter.attack_time = 0;
		info.parameter.flag = 0;

		user->current_player->set_defense_mission_info(info);

        return result;
	}

	std::uint32_t cmd_defense_mission_start::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
