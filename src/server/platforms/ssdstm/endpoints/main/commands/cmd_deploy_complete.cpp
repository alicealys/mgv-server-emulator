#include <std_include.hpp>

#include "cmd_deploy_complete.hpp"

#include "database/models/present_box.hpp"
#include "database/models/crew_members.hpp"

namespace emulator::ssd
{
	bool cmd_deploy_complete::complete_mission(const database::users::user& user, const database::deployments::deployment_team& team, const bool is_forced, json::value& result)
	{
		const auto& mission_map = database::deployments::get_deployments_mission_map();
		const auto iter = mission_map.find(team.get_mission_id());

		database::deployments::deploy_params_t end_deploy_params;
		end_deploy_params.crew_ids[0] = team.get_crew_id_01();
		end_deploy_params.crew_ids[1] = team.get_crew_id_02();
		end_deploy_params.crew_ids[2] = team.get_crew_id_03();
		end_deploy_params.crew_ids[3] = team.get_crew_id_04();
		end_deploy_params.name = team.get_info_name();
		end_deploy_params.combat = team.get_info_combat();
		end_deploy_params.survive = team.get_info_survive();
		end_deploy_params.status = database::deployments::team_status_deploy_none;
		end_deploy_params.team_params = {};
		end_deploy_params.mission_id = 0u;
		end_deploy_params.mission_info = 0u;
		end_deploy_params.mission_type = 0u;
		end_deploy_params.complete_date = std::chrono::system_clock::now();

		if (iter == mission_map.end())
		{
			team.deploy(end_deploy_params);
			return false;
		}

		auto& deploy_result = result["deploy_result"];

		const auto now = std::chrono::system_clock::now();
		const auto expire_date = now + 14 * 24h;
		const auto expire_date_s = std::chrono::duration_cast<std::chrono::seconds>(expire_date.time_since_epoch());

		auto& reward_info = deploy_result["reward_info"];
		reward_info["battle_pack_list"] = json::array();
		reward_info["expire_date"] = expire_date_s.count();
		reward_info["kub_boost_flag"] = 0;
		reward_info["nonstackable_list"] = json::array();
		reward_info["present_list"] = json::array();
		reward_info["recipe_list"] = json::array();
		reward_info["resources_list"] = json::array();
		reward_info["stackable_list"] = json::array();
		reward_info["text_id"] = 0;
		reward_info["energy"] = 0;

		deploy_result["bad_status_list"] = json::array();
		deploy_result["motivation_list"] = json::array();
		deploy_result["item_list"] = json::array();

		database::players::inventory_resource_list_t resource_list;
		database::players::stackable_item_list_t stackable_list;
		database::players::player_inventory_t player_inventory{};

		user.current_player->get_inventory_resource_list(resource_list);
		user.current_player->get_stackable_item_list(stackable_list);
		user.current_player->get_inventory(player_inventory);

		const auto& params = team.get_params();

		auto is_win = false;
		if (!is_forced)
		{
			if (team.get_info_status() == database::deployments::team_status_deploy_progress)
			{
				const auto rand = utils::cryptography::random::get_integer(0u, 100u);
				is_win = rand <= params.success_rate;
			}
			else
			{
				is_win = team.get_info_status() == database::deployments::team_status_deploy_success;
			}
		}

		if (is_win)
		{
			database::users::give_item_params_t give_params{};
			give_params.resource_list = &resource_list;
			give_params.stackable_list = &stackable_list;
			give_params.player_inventory = &player_inventory;

			for (auto i = 0ull; i < iter->second.reward_list.size(); i++)
			{
				const auto& reward = iter->second.reward_list[i];
				if (team.get_info_combat() < reward.combat_rate || team.get_info_survive() < reward.survival_rate)
				{
					continue;
				}

				if (!user.give_item(reward.item_info, give_params, reward_info))
				{
					database::present_box::add_item(user.current_player->get_player_id(),
						database::present_box::present_flag_expire | database::present_box::present_flag_new,
						expire_date_s, reward.item_info);
				}
			}
		}

		const auto crew_list = database::crew_members::get_all(user.current_player->get_player_id());
		for (auto i = 0ull; i < crew_list.size(); i++)
		{
			deploy_result["motivation_list"][i]["unique_id"] = crew_list[i].get_member_id();
			deploy_result["motivation_list"][i]["motivation"] = 0;
		}

		// TODO: injuries, damage items, ...

		for (auto i = 0; i < 5; i++)
		{
			team.get_item(i).to_json(deploy_result["item_list"][i]);
		}

		if (!team.deploy(end_deploy_params))
		{
			return false;
		}

		user.current_player->set_inventory_resource_list(resource_list);
		user.current_player->set_stackable_item_list(stackable_list);
		user.current_player->set_inventory(player_inventory);

		reward_info["present_box_num"] = database::present_box::get_present_count(user.current_player->get_player_id());
		return true;
	}

	json::value cmd_deploy_complete::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		std::uint64_t team_index{};
		if (!json::read(team_index, data["team_index"]))
		{
			return error(ERR_INVALIDARG);
		}

		const auto team = database::deployments::find_team_by_index(user->current_player->get_player_id(), team_index);
		if (!team.has_value())
		{
			return error(ERR_NOT_FOUND);
		}

		if (team->get_info_status() == database::deployments::team_status_deploy_none)
		{
			return error(ERR_NOT_DEPLOY);
		}

		if (team->get_info_status() == database::deployments::team_status_deploy_progress)
		{
			return error(ERR_NOT_COMPLETE);
		}

		if (!this->complete_mission(user.value(), team.value(), false, result))
		{
			return error(ERR_DATABASE);
		}

		return result;
	}

	std::uint32_t cmd_deploy_complete::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
