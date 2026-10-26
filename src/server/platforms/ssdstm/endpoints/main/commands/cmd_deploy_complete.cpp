#include <std_include.hpp>

#include "cmd_deploy_complete.hpp"

#include "database/models/present_box.hpp"
#include "database/models/crew_members.hpp"

#include "utils/random.hpp"
#include "utils/time.hpp"

namespace emulator::ssd
{
	bool cmd_deploy_complete::complete_mission(const database::users::user& user, const database::deployments::deployment_team& team, json::value& result)
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

		database::users::reward_info_t reward_info{};
		reward_info.expire_date = utils::time::from_now(14d);
		reward_info.text_id = game::lang_id_mission_completion_reward;

		deploy_result["bad_status_list"] = json::array();
		deploy_result["motivation_list"] = json::array();
		deploy_result["item_list"] = json::array();

		database::players::inventory_resource_list_t resource_list;
		database::players::stackable_item_list_t stackable_list;
		database::players::player_inventory_t player_inventory{};

		user.current_player->get_inventory_resource_list(resource_list);
		user.current_player->get_stackable_item_list(stackable_list);
		user.current_player->get_inventory(player_inventory);

		std::vector<game::item_t> presents;

		const auto& params = team.get_params();

		if (team.is_success())
		{
			database::users::give_item_params_t give_params{};
			give_params.resource_list = &resource_list;
			give_params.stackable_list = &stackable_list;
			give_params.player_inventory = &player_inventory;
			give_params.text_id = reward_info.text_id;

			for (auto i = 0ull; i < iter->second.reward_list.size(); i++)
			{
				const auto& reward = iter->second.reward_list[i];
				if (!params.has_reward(i) || !utils::random::chance(reward.rate))
				{
					continue;
				}

				if (!user.give_item(reward.item_info, give_params, &reward_info))
				{
					presents.emplace_back(reward.item_info);
					reward_info.present_list.emplace_back(reward.item_info);
				}
			}
		}
		
		const auto is_deploy_crew = [&](const std::uint64_t member_id, std::size_t& index)
		{
			for (auto i = 0ull; i < end_deploy_params.crew_ids.size(); i++)
			{
				if (end_deploy_params.crew_ids[i] == member_id)
				{
					index = i;
					return true;
				}
			}

			return false;
		};

		auto& bad_status_list_j = deploy_result["bad_status_list"];
		auto& motivation_list_j = deploy_result["motivation_list"];

		const auto crew_list = database::crew_members::get_all(user.current_player->get_player_id());
		for (auto i = 0ull; i < crew_list.size(); i++)
		{
			auto& motivation_entry = motivation_list_j[motivation_list_j.size()];
			motivation_entry["unique_id"] = crew_list[i].get_member_id();
			motivation_entry["motivation"] = crew_list[i].motivation_history.value[0];

			std::size_t crew_index{};
			if (!is_deploy_crew(crew_list[i].get_member_id(), crew_index))
			{
				continue;
			}

			if (utils::random::chance(params.injure_rate))
			{
				const auto new_life = crew_list[i].get_life();
				auto recovery_time = utils::random::get_integer(1000u, 5000u);
				auto bad_status_type = 0u;

				const auto is_injury = utils::random::cointoss() &&
					(crew_list[i].get_injury_id_1() == 0 ||
					crew_list[i].get_injury_id_2() == 0);

				if (is_injury)
				{
					const auto injury_index = utils::random::index(database::crew_members::injury_list);
					const auto& injury = database::crew_members::injury_list[injury_index];
					bad_status_type = injury.type_index;

					if (crew_list[i].get_injury_id_1() == 0)
					{
						crew_list[i].update_injury(new_life,
							bad_status_type, crew_list[i].get_injury_id_2(),
							recovery_time, crew_list[i].get_injury_time_2()
						);
					}
					else if (crew_list[i].get_injury_id_2() == 0)
					{
						crew_list[i].update_injury(new_life,
							crew_list[i].get_injury_id_1(), bad_status_type,
							crew_list[i].get_injury_time_1(), recovery_time
						);
					}
					else
					{
						bad_status_type = 99u;
					}
				}
				else
				{
					const auto sickness_index = utils::random::index(database::crew_members::sickness_list);
					const auto& sickness = database::crew_members::sickness_list[sickness_index];
					bad_status_type = sickness.type_index;

					if (crew_list[i].get_sickness_id_1() == 0)
					{
						crew_list[i].update_sickness(new_life,
							bad_status_type, crew_list[i].get_sickness_id_2(),
							recovery_time, crew_list[i].get_sickness_time_2()
						);
					}
					else if (crew_list[i].get_sickness_id_2() == 0)
					{
						crew_list[i].update_sickness(new_life,
							crew_list[i].get_sickness_id_1(), bad_status_type,
							crew_list[i].get_sickness_time_1(), recovery_time
						);
					}
					else
					{
						bad_status_type = 99u;
					}
				}

				auto& status = bad_status_list_j[bad_status_list_j.size()];
				status["bad_status_type"] = bad_status_type;
				status["recovery_time"] = recovery_time;
				status["unique_id"] = crew_list[i].get_member_id();

				if (bad_status_type == 99u)
				{
					end_deploy_params.crew_ids[crew_index] = 0;
					database::deployments::remove_crew_id(user.current_player->get_player_id(), crew_list[i].get_member_id());
					database::crew_members::remove(user.current_player->get_player_id(), crew_list[i].get_member_id());
				}
			}
		}

		constexpr const auto time_factor_scale = 2400.f;
		constexpr const std::uint16_t item_life_step = 20u;

		const auto time_factor = static_cast<float>(iter->second.required_time) / time_factor_scale;
		auto survival_cost = static_cast<std::int32_t>(static_cast<float>(iter->second.survival_penalty) * time_factor);
		auto combat_cost = static_cast<std::int32_t>(static_cast<float>(iter->second.combat_penalty) * time_factor);

		struct item_info_t
		{
			database::deployments::team_item_t item;
			std::uint32_t sup_combat;
			std::uint32_t sup_survival;
			std::uint16_t life;
		};

		std::array<item_info_t, 5> items{};
		for (auto i = 0ull; i < items.size(); i++)
		{
			items[i].item = team.get_item(static_cast<std::uint32_t>(i));

			auto& item_data = items[i].item;
			switch (item_data.f.type)
			{
			case database::deployments::item_type_stackable:
			{
				if (item_data.f.id_index >= game::parameters_table.ssd_sbm_parameters->productions_list.size())
				{
					break;
				}

				const auto& prod = game::parameters_table.ssd_sbm_parameters->productions_list[item_data.f.id_index];
				if (prod == nullptr)
				{
					break;
				}

				items[i].sup_combat = prod->sup_combat;
				items[i].sup_survival = prod->sup_survival;
				break;
			}
			case database::deployments::item_type_resource:
			{
				if (item_data.f.id_index >= game::parameters_table.ssd_sbm_parameters->resources_list.size())
				{
					break;
				}

				const auto& prod = game::parameters_table.ssd_sbm_parameters->resources_list[item_data.f.id_index];
				if (prod == nullptr)
				{
					break;
				}

				items[i].sup_combat = prod->sup_combat;
				items[i].sup_survival = prod->sup_survival;
				break;
			}
			case database::deployments::item_type_nonstackable:
			{
				if (item_data.f.id_index >= game::parameters_table.ssd_sbm_parameters->productions_list.size())
				{
					break;
				}

				const auto& prod = game::parameters_table.ssd_sbm_parameters->productions_list[item_data.f.id_index];
				if (prod == nullptr)
				{
					break;
				}

				items[i].sup_combat = prod->sup_combat;
				items[i].sup_survival = prod->sup_survival;
				items[i].life = static_cast<std::uint16_t>(prod->life);
				break;
			}
			}
		}

		const auto do_damage = [&](std::int32_t& target_cost, const bool is_combat)
		{
			auto loop_max = 100u;
			auto target_item_index = 0u;
			while (target_cost > 0 && --loop_max > 0)
			{
				auto search_count = 5;
				item_info_t* target_item = nullptr;

				while (search_count-- > 0)
				{
					auto& item = items[target_item_index % 5];
					if (item.item.f.param1 <= 0)
					{
						++target_item_index;
						continue;
					}

					if ((is_combat && item.sup_combat > 0) || (!is_combat && item.sup_survival > 0))
					{
						target_item = &item;
						break;
					}

					++target_item_index;
				}

				if (target_item == nullptr)
				{
					break;
				}

				switch (target_item->item.f.type)
				{
				case database::deployments::item_type_stackable:
				case database::deployments::item_type_resource:
				{
					target_cost -= is_combat ? target_item->sup_combat : target_item->sup_survival;
					target_item->item.f.param1--;
					break;
				}
				case database::deployments::item_type_nonstackable:
				{
					const auto step = std::min(target_item->item.f.param1, item_life_step);
					const auto frac = static_cast<float>(step) / static_cast<float>(target_item->life);
					const auto sup_combat = static_cast<std::int32_t>(static_cast<float>(target_item->sup_combat) * frac);
					const auto sup_survival = static_cast<std::int32_t>(static_cast<float>(target_item->sup_survival) * frac);

					target_cost -= is_combat ? sup_combat : sup_survival;
					target_item->item.f.param1 -= step;
					break;
				}
				}

				++target_item_index;
			}
		};

		do_damage(survival_cost, false);
		do_damage(combat_cost, true);

		for (auto i = 0; i < 5; i++)
		{
			auto& item = items[i];
			if (item.item.f.param1 == 0)
			{
				item.item = {};
			}

			team.update_item(i, items[i].item);
			item.item.to_json(deploy_result["item_list"][i]);
		}

		if (!team.deploy(end_deploy_params))
		{
			return false;
		}

		user.current_player->set_inventory_resource_list(resource_list);
		user.current_player->set_stackable_item_list(stackable_list);
		user.current_player->set_inventory(player_inventory);

		database::present_box::add_items(user.get_user_id(),
			database::present_box::present_flag_expire, 
			reward_info.expire_date, presents, reward_info.text_id);

		reward_info.present_box_num = database::present_box::get_present_count(user.get_user_id());
		reward_info.to_json(deploy_result["reward_info"]);

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

		if (team->is_progress() && !team->is_complete())
		{
			return error(ERR_NOT_COMPLETE);
		}

		if (!this->complete_mission(user.value(), team.value(), result))
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
