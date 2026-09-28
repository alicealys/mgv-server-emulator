#include <std_include.hpp>

#include "cmd_deploy_complete.hpp"

#include "database/models/deployments.hpp"
#include "database/models/present_box.hpp"

namespace emulator::ssd
{
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

		const auto now = std::chrono::system_clock::now();
		if (team->get_info_status() == database::deployments::team_status_deploy_progress)
		{
			return error(ERR_NOT_COMPLETE);
		}

		const auto& mission_map = database::deployments::get_deployments_mission_map();
		const auto iter = mission_map.find(team->get_mission_id());
		if (iter == mission_map.end())
		{
			return error(ERR_DATABASE);
		}

		auto& deploy_result = result["deploy_result"];

		const auto expire_date = now + 14 * 24h;
		const auto expire_date_s = std::chrono::duration_cast<std::chrono::seconds>(expire_date.time_since_epoch());

		auto& reward_info = deploy_result["reward_info"];
		reward_info["battle_pack_list"] = json::array();
		reward_info["expire_date"] = expire_date_s.count();
		reward_info["kub_boost_flag"] = 0;
		reward_info["nonstackable_list"] = json::array();
		reward_info["present_num"] = 0;
		reward_info["present_list"] = json::array();
		reward_info["recipe_list"] = json::array();
		reward_info["resources_list"] = json::array();
		reward_info["stackable_list"] = json::array();
		reward_info["text_id"] = 229691447841577;
		reward_info["energy"] = 0;

		deploy_result["bad_status_list"] = json::array();
		deploy_result["motivation_list"] = json::array();
		deploy_result["item_list"] = json::array();

		database::players::inventory_resource_list_t resource_list;
		database::players::stackable_item_list_t stackable_list;
		database::players::player_inventory_t inventory_info{};

		user->current_player->get_inventory_resource_list(resource_list);
		user->current_player->get_stackable_item_list(stackable_list);
		user->current_player->get_inventory(inventory_info);

		const auto& params = team->get_params();

		auto is_win = false;
		if (team->get_info_status() == database::deployments::team_status_deploy_progress)
		{
			const auto rand = utils::cryptography::random::get_integer(0u, 100u);
			is_win = rand <= params.success_rate;
		}
		else
		{
			is_win = team->get_info_status() == database::deployments::team_status_deploy_success;
		}

		if (is_win)
		{
			for (auto i = 0ull; i < iter->second.reward_list.size(); i++)
			{
				const auto& reward = iter->second.reward_list[i];
				if (team->get_info_combat() < reward.combat_rate || team->get_info_survive() < reward.survival_rate)
				{
					continue;
				}

				if (!user->current_player->give_reward(reward.item_info, &stackable_list, &resource_list, 
					&inventory_info, reward_info))
				{
					database::present_box::add_item(user->current_player->get_player_id(),
						database::present_box::present_flag_expire | database::present_box::present_flag_new,
						expire_date_s, reward.item_info);
				}
			}
		}

		// TODO: injuries, damage items, ...

		for (auto i = 0; i < 5; i++)
		{
			team->get_item(i).to_json(deploy_result["item_list"][i]);
		}
		
		database::deployments::team_params_t new_params{};

		team->update_params(new_params);
		team->update_status(database::deployments::team_status_deploy_none);
		team->update_mission(0, 0, 0, 0);

		user->current_player->set_inventory_resource_list(resource_list);
		user->current_player->set_stackable_item_list(stackable_list);
		user->current_player->set_inventory(inventory_info);

		reward_info["present_num"] = database::present_box::get_present_count(user->current_player->get_player_id());

		return result;
	}

	std::uint32_t cmd_deploy_complete::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
