#include <std_include.hpp>

#include "cmd_deploy_get_list.hpp"

#include "database/models/deployments.hpp"

namespace emulator::ssd
{
	json::value cmd_deploy_get_list::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		param_t param{};
		if (!json::read(param, data))
		{
			return error(ERR_INVALIDARG);
		}

		std::unordered_set<std::uint32_t> active_missions;

		const auto teams = database::deployments::get_all_teams(user->current_player->get_player_id());
		for (auto i = 0ull; i < teams.size(); i++)
		{
			if (teams[i].get_info_status() == database::deployments::team_status_deploy_progress)
			{
				active_missions.insert(teams[i].get_mission_id());
			}
		}

		result["mission_list"] = json::array();
		auto count = 0u;

		const auto& mission_list = database::deployments::get_deployments_mission_list();
		for (auto i = 0ull; i < mission_list.size(); i++)
		{
			if (active_missions.contains(mission_list[i].id))
			{
				auto& entry = result["mission_list"][count++];
				mission_list[i].to_json(entry);
				entry["inactive"] = 1;
				entry["is_new"] = 0;
				continue;
			}

			if (param.level < mission_list[i].difficulty + 1)
			{
				continue;
			}

			mission_list[i].to_json(result["mission_list"][count++]);
		}

		return result;
	}

	std::uint32_t cmd_deploy_get_list::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
