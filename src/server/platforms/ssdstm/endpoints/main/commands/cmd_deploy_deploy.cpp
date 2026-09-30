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

		if (team->get_info_status() != database::deployments::team_status_deploy_none)
		{
			return error(ERR_ALREADY_DEPLOY);
		}

		if (team->get_info_combat() < iter->second.combat || team->get_info_survive() < iter->second.survival)
		{
			return error(ERR_SHORTAGE);
		}

		database::deployments::deploy_params_t deploy_params;
		deploy_params.crew_ids[0] = param.team_info.crew_id_list[0];
		deploy_params.crew_ids[1] = param.team_info.crew_id_list[1];
		deploy_params.crew_ids[2] = param.team_info.crew_id_list[2];
		deploy_params.crew_ids[3] = param.team_info.crew_id_list[3];
		deploy_params.name = utils::cryptography::base64::decode(param.team_info.name);
		deploy_params.combat = param.team_info.combat;
		deploy_params.survive = param.team_info.survive;
		deploy_params.status = database::deployments::team_status_deploy_progress;
		deploy_params.team_params = team_params;
		deploy_params.mission_id = param.mission_id;
		deploy_params.mission_info = param.mission_info;
		deploy_params.mission_type = param.mission_type;
		deploy_params.complete_date = std::chrono::system_clock::now() + param.required_time * 1s;

		if (!team->deploy(deploy_params))
		{
			return error(ERR_DATABASE);
		}

		result["team_status"] = deploy_params.status;
		result["team_index"] = team->get_index();
		result["complete_time"] = std::chrono::duration_cast<std::chrono::seconds>(deploy_params.complete_date.time_since_epoch()).count();

		return result;
	}

	std::uint32_t cmd_deploy_deploy::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
