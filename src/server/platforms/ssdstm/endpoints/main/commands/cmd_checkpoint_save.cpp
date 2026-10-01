#include <std_include.hpp>

#include "cmd_checkpoint_save.hpp"
#include "cmd_inventory_save.hpp"

#include "database/models/present_box.hpp"
#include "database/models/crew_members.hpp"

namespace emulator::ssd
{
	void cmd_checkpoint_save::generate_defense_mission_rewards(
		json::value& result, const std::optional<database::users::user>& user,
		database::defense_missions::reward_list_t& reward_list,
		const game::defense_mission_settings_t& mission_settings, const std::uint8_t rank)
	{
		database::players::stackable_item_list_t stackable_list{};
		database::players::nonstackable_item_list_t nonstackable_list{};
		database::players::inventory_resource_list_t resource_list{};
		database::players::player_inventory_t player_inventory{};

		const auto rewards = database::defense_missions::generate_rewards(mission_settings, rank);
		if (!rewards.empty())
		{
			user->current_player->get_stackable_item_list(stackable_list);
			user->current_player->get_nonstackable_item_list(nonstackable_list);
			user->current_player->get_inventory_resource_list(resource_list);
			user->current_player->get_inventory(player_inventory);
		}

		for (const auto& reward : rewards)
		{
			reward_list.push(reward.param);
		}

		const auto now = std::chrono::system_clock::now();
		const auto expire_date = now + 14 * 24h;
		const auto expire_date_s = std::chrono::duration_cast<std::chrono::seconds>(expire_date.time_since_epoch());

		for (auto i = rank; i <= 5; i++)
		{
			auto& rank_entry = result[i - rank];
			rank_entry["battle_pack_list"] = json::array();
			rank_entry["expire_date"] = expire_date_s.count();
			rank_entry["kub_boost_flag"] = 0;
			rank_entry["nonstackable_list"] = json::array();
			rank_entry["present_num"] = 0;
			rank_entry["present_list"] = json::array();
			rank_entry["recipe_list"] = json::array();
			rank_entry["resources_list"] = json::array();
			rank_entry["stackable_list"] = json::array();
			rank_entry["text_id"] = 229691447841577;
			rank_entry["energy"] = 0;
		}
		
		// todo: give recipes?

		database::users::give_item_params_t give_params{};
		give_params.resource_list = &resource_list;
		give_params.stackable_list = &stackable_list;
		give_params.nonstackable_list = &nonstackable_list;
		give_params.player_inventory = &player_inventory;

		for (const auto& reward : rewards)
		{
			if (reward.rank < rank)
			{
				continue;
			}

			auto& rank_entry = result[reward.rank - rank];
			auto& present_list_j = rank_entry["present_list"];

			if (!user->give_item(reward.param, give_params, rank_entry))
			{
				database::present_box::add_item(user->current_player->get_player_id(),
					database::present_box::present_flag_expire | database::present_box::present_flag_new, expire_date_s, reward.param);
				reward.param.to_json(present_list_j[present_list_j.size()]);
			}
		}

		const auto present_count = database::present_box::get_present_count(user->current_player->get_player_id());
		for (auto i = rank; i <= 5; i++)
		{
			auto& rank_entry = result[i - rank];
			rank_entry["present_num"] = present_count;
		}

		if (!rewards.empty())
		{
			user->current_player->set_inventory_resource_list(resource_list);
			user->current_player->set_stackable_item_list(stackable_list);
			user->current_player->set_nonstackable_item_list(nonstackable_list);
			user->current_player->set_inventory(player_inventory);
		}
	}

	void cmd_checkpoint_save::save_defense_mission(json::value& result, json::value& wave_results, const std::optional<database::users::user>& user)
	{
		const auto now = std::chrono::system_clock::now().time_since_epoch();
		const auto mission = database::defense_missions::get_current_mission(user->current_player->get_player_id());

		if (!mission.has_value())
		{
			return;
		}

		const auto is_in_interval = now < mission->get_next_wave_date();
		const auto iter = game::parameters_table.ssd_base_defense_settings->mission_settings.find(mission->get_mission_code());
		if (iter == game::parameters_table.ssd_base_defense_settings->mission_settings.end())
		{
			return;
		}

		auto defense_mission_info = user->current_player->get_defense_mission_info();

		const auto _0 = gsl::finally([&]
		{
			user->current_player->set_defense_mission_info(defense_mission_info);
		});

		result["defense_reward_limit_result"]["limit_result"] = json::array();
		result["defense_reward_limit_result"]["mission_code"] = mission->get_mission_code();

		auto& defense_mission_reward_j = result["defense_mission_reward"];
		defense_mission_reward_j = json::array();

		auto& mission_settings = iter->second;
		for (auto i = 0ull; i < wave_results.size(); i++)
		{
			auto& entry = wave_results[i];

			database::defense_missions::wave_result_t wave_result{};
			database::defense_missions::wave_params_t wave_params{};

			if (!json::read(wave_result, entry) || !wave_params.parse(entry) || (is_in_interval && wave_result.result != 3))
			{
				continue;
			}

			wave_result.wave = mission->get_current_wave();

			const auto is_win = wave_result.result == 1 || wave_result.result == 0;
			const auto score_add = is_win ? wave_result.total_score : 0u;
			const auto new_score = mission->get_total_score() + score_add;

			defense_mission_info.status.mining_machine_life = wave_params.mining_machine_life_after;

			const auto this_wave = mission->get_current_wave();
			const auto rank = database::defense_missions::calc_rank(*mission_settings, new_score);

			std::uint8_t next_wave = mission->get_current_wave();
			std::uint8_t mission_result = 0u;

			auto end_date = std::chrono::seconds(wave_result.end_date);
			auto next_wave_date = is_in_interval ? mission->get_next_wave_date() : end_date;

			database::defense_missions::reward_list_t reward_list{};

			switch (wave_result.result)
			{
			case 0: // wave success
			case 1: // wave success + mission cleared
			{
				if (this_wave == mission_settings->max_wave_count - 1)
				{
					mission_result = 1u;
					this->generate_defense_mission_rewards(defense_mission_reward_j, user, reward_list, *mission_settings, rank);
				}
				else
				{
					next_wave++;
					mission_result = 0u;
					next_wave_date = std::chrono::duration_cast<std::chrono::seconds>(now);
					if (this_wave < mission_settings->interval.size())
					{
						next_wave_date += mission_settings->interval[this_wave] * 1h;
					}
				}

				break;
			}
			case 2: // wave fail
				mission_result = 0u;
				break;
			case 3: // suspend mission
				mission_result = 2u;
				this->generate_defense_mission_rewards(defense_mission_reward_j, user, reward_list, *mission_settings, rank);
				break;
			}

			mission->update(mission_result, next_wave, rank, new_score, end_date, next_wave_date);

			database::defense_missions::injury_crew_list_t injury_crew_list{};
			database::defense_missions::broken_facility_list_t broken_facility_list{};

			injury_crew_list.parse(entry["injury_crew_list"]);
			broken_facility_list.parse(entry["broken_facility_list"]);

			database::defense_missions::add_wave(mission->get_defense_mission_id(),
				wave_result, wave_params, broken_facility_list, injury_crew_list, reward_list);
		}
	}

	json::value cmd_checkpoint_save::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		cmd_inventory_save::do_save(data, user);

		auto& gimmick_timer_info_j = data["gimmick_timer_info"];
		auto& gimmick_save_info_j = data["gimmick_save_info"];
		auto& mission_info_j = data["current_mission_info"];
		auto& base_resource_j = data["base_resource"];
		auto& base_resource_count_j = data["base_resource_count"];
		auto& animal_list_j = data["animal_list"];
		auto& story_unlock_info_j = data["story_unlock_info"];
		auto& open_list_j = data["open_list"];
		auto& reward_crew_j = data["reward_crew"];
		auto& crew_update_list_j = data["crew_update_list"];
		auto& crew_died_list_j = data["crew_died_list"];
		auto& group_level_j = data["group_level"];
		auto& tips_open_info_j = data["tips_open_info"];
		auto& quest_record_info_j = data["quest_record_info"];
		auto& defense_mission_wave_result_j = data["defense_mission_wave_result"];
		auto& defense_mission_parameter_j = data["defense_mission_parameter"];

		result["added_crew"] = json::array();
		result["capture_list"] = json::array();
		result["defense_mission_reward"] = json::array();
		result["left_resources"] = json::array();
		result["stackable_list"] = json::array();
		result["reward_event_point"] = 0;
		result["boost_flag"] = 0;
		result["defense_reward_limit_result"]["limit_result"] = json::array();
		result["defense_reward_limit_result"]["mission_code"] = 0;

		if (gimmick_save_info_j.is_object())
		{
			auto& map_location_j = gimmick_save_info_j["map_location"];
			if (!map_location_j.is_uint64())
			{
				return error(ERR_INVALIDARG);
			}

			auto gimmick_info = user->current_player->get_gimmick_info();

			const auto map_location = map_location_j.as<std::uint32_t>();
			if (map_location > 1)
			{
				return error(ERR_INVALIDARG);
			}

			switch (map_location)
			{
			case 0:
				gimmick_info.resource_afghan.parse(gimmick_timer_info_j);
				break;
			case 1:
				gimmick_info.resource_africa.parse(gimmick_timer_info_j);
				break;
			}

			if (gimmick_timer_info_j.is_object())
			{
				gimmick_info.timer.parse(gimmick_timer_info_j);
			}

			user->current_player->set_gimmick_info(gimmick_info);

			const auto gimmick_data = std::make_unique<database::players::gimmick_save_data_t>();
			gimmick_data->parse(gimmick_save_info_j);
			user->current_player->set_gimmick_save_data(*gimmick_data, map_location);
		}

		const auto base_resources = std::make_unique<database::players::base_resources_t>();
		user->current_player->get_base_resources(*base_resources);

		if (base_resource_count_j.is_array() && base_resource_count_j.size())
		{
			base_resources->parse_counts(base_resource_count_j[0]);
		}

		if (base_resource_j.is_array() && base_resource_j.size())
		{
			base_resources->parse_base(base_resource_j[0]);
		}

		if (animal_list_j.is_array() && animal_list_j.size())
		{
			base_resources->parse_animals(animal_list_j[0]);
		}

		user->current_player->set_base_resources(*base_resources);

		const auto parse_map_unlock_list = [&](json::value& story_unlock_info_j)
		{
			auto& map_unlock_list_j = story_unlock_info_j["map_unlock_list"];
			if (!map_unlock_list_j.is_array())
			{
				return;
			}

			for (auto i = 0ull; i < map_unlock_list_j.size(); i++)
			{
				auto& entry = map_unlock_list_j[i];
				if (!entry.is_object())
				{
					continue;
				}

				auto& location_index_j = entry["location_index"];
				auto& map_unlock_j = entry["map_unlock"];
				if (!location_index_j.is_uint64() || !map_unlock_j.is_array() || map_unlock_j.size() == 0)
				{
					continue;
				}

				const auto location_index = location_index_j.as<std::uint32_t>();
				const auto map_unlock_list = std::make_unique<database::players::map_unlock_list_t>();

				switch (location_index)
				{
				case 0:
					user->current_player->get_map_unlock_list_afghan(*map_unlock_list);
					map_unlock_list->parse_diff(map_unlock_j);
					user->current_player->set_map_unlock_list_afghan(*map_unlock_list);
					break;
				case 1:
					user->current_player->get_map_unlock_list_africa(*map_unlock_list);
					map_unlock_list->parse_diff(map_unlock_j);
					user->current_player->set_map_unlock_list_africa(*map_unlock_list);
					break;
				}
			}
		};

		const auto story_unlock_info = std::make_unique<database::players::story_unlock_info_t>();

		if (story_unlock_info_j.is_object())
		{
			user->current_player->get_story_unlock_info(*story_unlock_info);
			story_unlock_info->parse(story_unlock_info_j);
			parse_map_unlock_list(story_unlock_info_j);
		}

		if (mission_info_j.is_object())
		{
			const auto mission_info = std::make_unique<database::players::mission_info_t>();
			user->current_player->get_mission_info(*mission_info);

			if (!mission_info->parse(mission_info_j))
			{
				return error(ERR_INVALIDARG);
			}

			user->current_player->set_mission_info(*mission_info);
		}

		if (open_list_j.is_object())
		{
			auto& mission_code_list_j = open_list_j["mission_code_list"];
			if (mission_code_list_j.is_array())
			{
				 auto mission_record_list = user->current_player->get_mission_record_list();

				for (auto i = 0ull; i < mission_code_list_j.size(); i++)
				{
					auto& mission_code_j = mission_code_list_j[i];
					if (!mission_code_j.is_uint64())
					{
						continue;
					}

					const auto mission_code = mission_code_j.as<std::uint16_t>();
					mission_record_list.open_mission(mission_code);
				}

				user->current_player->set_mission_record_list(mission_record_list);
			}

			auto& quest_mission_code_list_j = open_list_j["quest_mission_code_list"];
			if (quest_mission_code_list_j.is_array())
			{
				 auto quest_record_list = user->current_player->get_quest_record_list();
				quest_record_list.parse_diff(quest_mission_code_list_j);
				user->current_player->set_quest_record_list(quest_record_list);
			}
		}

		if (quest_record_info_j.is_array())
		{
			for (auto i = 0ull; i < quest_record_info_j.size(); i++)
			{
				auto quest_record_list = user->current_player->get_quest_record_list();
				quest_record_list.parse_diff(quest_record_info_j);
				user->current_player->set_quest_record_list(quest_record_list);
			}
		}

		if (reward_crew_j.is_array())
		{
			auto count = 0u;

			for (auto i = 0ull; i < reward_crew_j.size(); i++)
			{
				database::crew_members::member_params_t new_member_params{};
				if (!new_member_params.parse_add_param(reward_crew_j[i]))
				{
					continue;
				}

				const auto member_count = database::crew_members::get_member_count(user->current_player->get_player_id());
				if (member_count >= database::crew_members::max_crew_members)
				{
					return error(ERR_OVER_CAPACITY);
				}

				const auto added_member_id = database::crew_members::create(user->current_player->get_player_id(), new_member_params);
				if (added_member_id == 0ull)
				{
					return error(ERR_DATABASE);
				}

				const auto new_member = database::crew_members::find(user->current_player->get_player_id(), added_member_id);
				new_member->to_json(result["added_crew"][count++]);
			}
		}

		if (crew_update_list_j.is_array() || crew_died_list_j.is_array())
		{
			if (crew_update_list_j.is_array())
			{
				for (auto i = 0ull; i < crew_update_list_j.size(); i++)
				{
					database::crew_members::member_params_t update_params{};
					if (!update_params.parse_update(crew_update_list_j[i]))
					{
						continue;
					}

					const auto member = database::crew_members::find(user->current_player->get_player_id(), update_params.unique_index);
					if (!member.has_value())
					{
						continue;
					}

					member->update(update_params);
				}
			}

			if (crew_died_list_j.is_array())
			{
				for (auto i = 0ull; i < crew_died_list_j.size(); i++)
				{
					std::uint64_t unique_id{};
					if (!json::read(unique_id, crew_died_list_j[i]))
					{
						continue;
					}

					database::crew_members::remove(user->current_player->get_player_id(), unique_id);
				}
			}
		}

		if (group_level_j.is_object())
		{
			database::players::crew_levels_t levels{};
			levels.parse(group_level_j);
			user->current_player->set_crew_levels(levels);
		}

		if (tips_open_info_j.is_array() && tips_open_info_j.size())
		{
			story_unlock_info->tips_open_info.parse(tips_open_info_j[0]);
		}

		user->current_player->set_story_unlock_info(*story_unlock_info);

		if (defense_mission_wave_result_j.is_array())
		{
			this->save_defense_mission(result, defense_mission_wave_result_j, user);
		}

		if (defense_mission_parameter_j.is_object())
		{
			auto info = user->current_player->get_defense_mission_info();
			info.parameter.parse(defense_mission_parameter_j);
			user->current_player->set_defense_mission_info(info);
		}

		// TODO
		// play_record_additional_130_checkpoint
		// play_record_save_checkpoint
		// replay_mission_info
		// quest_repop_count_decrement

		return result;
	}

	std::uint32_t cmd_checkpoint_save::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
