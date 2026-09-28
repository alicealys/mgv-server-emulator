#include <std_include.hpp>

#include "cmd_deploy_get_list.hpp"

#include "database/models/deployments.hpp"

namespace emulator::ssd
{
	json::value cmd_deploy_get_list::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		const auto teams = database::deployments::get_all_teams(user->current_player->get_player_id());
		auto max_combat = 0u;
		auto max_survive = 0u;

		for (auto i = 0ull; i < teams.size(); i++)
		{
			max_combat = std::max(max_combat, teams[i].get_info_combat());
			max_survive = std::max(max_survive, teams[i].get_info_survive());
		}

		result["mission_list"] = json::array();
		auto count = 0u;

		const auto& mission_list = database::deployments::get_deployments_mission_list();
		for (auto i = 0ull; i < mission_list.size(); i++)
		{
			if (max_combat < mission_list[i].combat || max_survive < mission_list[i].survival)
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
