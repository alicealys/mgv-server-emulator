#include <std_include.hpp>

#include "cmd_deploy_deploy.hpp"

#include "database/models/deployments.hpp"

namespace emulator::ssd
{
	json::value cmd_deploy_deploy::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		param_t param{};
		if (!json::read(param, data))
		{
			return error(ERR_INVALIDARG);
		}

		database::deployments::team_params_t team_params{};
		team_params.success_rate = param.success_rate;
		team_params.injure_rate = param.injure_rate;
		team_params.use_fast_travel = param.use_fast_travel;
		team_params.required_combat = param.required_combat;
		team_params.required_survive = param.required_survive;
		team_params.required_time = param.required_time;

		const auto& mission_map = database::deployments::get_deployments_mission_map();
		const auto iter = mission_map.find(param.mission_id);
		if (iter == mission_map.end())
		{
			return error(ERR_NOT_FOUND);
		}

		const auto team = database::deployments::find_team_by_index(user->current_player->get_player_id(), param.team_info.index);
		if (!team.has_value())
		{
			return error(ERR_NOT_FOUND);
		}

		const auto now = std::chrono::system_clock::now();
		if (team->get_info_status() != database::deployments::team_status_deploy_none)
		{
			return error(ERR_ALREADY_DEPLOY);
		}

		if (team->get_info_combat() < iter->second.combat || team->get_info_survive() < iter->second.survival)
		{
			return error(ERR_SHORTAGE);
		}

		team->update_crew_ids(
			param.team_info.crew_id_list[0], param.team_info.crew_id_list[1],
			param.team_info.crew_id_list[2], param.team_info.crew_id_list[3]);

		const auto name = utils::cryptography::base64::decode(param.team_info.name);
		team->update_info(param.team_info.combat, name, param.team_info.survive);
		team->update_status(database::deployments::team_status_deploy_progress);
		team->update_params(team_params);
		team->update_mission(param.mission_id, param.mission_info, param.mission_type, param.required_time);

		const auto updated_team = database::deployments::find_team_by_index(user->current_player->get_player_id(), param.team_info.index);
		if (!updated_team.has_value())
		{
			return error(ERR_DATABASE);
		}

		result["team_status"] = updated_team->get_info_status();
		result["team_index"] = updated_team->get_index();
		result["complete_time"] = updated_team->get_complete_date().count();

		return result;
	}

	std::uint32_t cmd_deploy_deploy::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
