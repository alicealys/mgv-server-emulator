#include <std_include.hpp>

#include "cmd_checkpoint_save.hpp"
#include "cmd_inventory_save.hpp"

#include "database/models/present_box.hpp"
#include "database/models/crew_members.hpp"
#include "database/models/challenge_tasks.hpp"
#include "database/models/boosts.hpp"
#include "database/models/rankings.hpp"
#include "database/models/events.hpp"

namespace emulator::ssd
{
	void cmd_checkpoint_save::generate_defense_mission_rewards(
		json::value& result, const std::optional<database::users::user>& user,
		database::defense_missions::reward_list_t& reward_list,
		const game::defense_mission_settings_t& mission_settings, const std::uint8_t rank)
	{
		std::uint32_t reward_event_point{};
		const auto rewards = database::defense_missions::generate_rewards(mission_settings, rank, reward_event_point);

		database::events::cached_event.access([&](const std::optional<database::events::running_event>& event)
		{
			if (!event.has_value() || !event->params->has_mission(mission_settings.mission_id))
			{
				reward_event_point = 0u;
			}
		});

		const auto boost_list = database::boosts::get_boost_list(user->get_user_id());
		const auto& kub_boost = boost_list[database::boosts::boost_type_kub];
		const auto& bp_boost = boost_list[database::boosts::boost_type_bp];
		const auto has_boost = bp_boost.valid || kub_boost.valid;

		result["boost_flag"] = has_boost & 1;

		if (reward_event_point > 0)
		{
			if (bp_boost.valid)
			{
				reward_event_point *= bp_boost.get_multiplier();
			}

			user->add_event_points(reward_event_point);
			database::rankings::increment_points(user->get_user_id(), database::rankings::ranking_type_event, reward_event_point);
			result["reward_event_point"] = reward_event_point;
		}

		if (rewards.empty())
		{
			return;
		}

		database::players::stackable_item_list_t stackable_list{};
		database::players::nonstackable_item_list_t nonstackable_list{};
		database::players::inventory_resource_list_t resource_list{};
		database::players::player_inventory_t player_inventory{};
		const auto user_inventory = std::make_unique<database::users::user_inventory_t>();

		user->current_player->get_stackable_item_list(stackable_list);
		user->current_player->get_nonstackable_item_list(nonstackable_list);
		user->current_player->get_inventory_resource_list(resource_list);
		user->current_player->get_inventory(player_inventory);
		user->get_inventory(*user_inventory);

		for (const auto& reward : rewards)
		{
			reward_list.push(reward.param);
		}

		std::vector<database::users::reward_info_t> rank_reward_infos(mission_settings.rank_threshold.size());
		auto& defense_mission_reward_j = result["defense_mission_reward"];

		const auto expire_date = utils::time::from_now(14d);
		constexpr const auto text_id = game::lang_id_mission_completion_reward;

		database::users::give_item_params_t give_params{};
		give_params.resource_list = &resource_list;
		give_params.stackable_list = &stackable_list;
		give_params.nonstackable_list = &nonstackable_list;
		give_params.player_inventory = &player_inventory;
		give_params.user_inventory = user_inventory.get();
		give_params.text_id = text_id;

		std::vector<game::item_t> presents;

		for (const auto& reward : rewards)
		{
			if (reward.rank >= rank_reward_infos.size())
			{
				continue;
			}

			auto& rank_entry = rank_reward_infos[reward.rank];
			rank_entry.text_id = text_id;
			rank_entry.expire_date = expire_date;

			game::item_t give_item = reward.param;
			if (kub_boost.valid && give_item.category == game::ITEM_CATEGORY_ENERGY)
			{
				const auto multiplier = (static_cast<float>(kub_boost.get_multiplier()) / 2.f) * 0.1f;
				give_item.num += static_cast<std::uint32_t>(static_cast<float>(give_item.num) * multiplier);
				rank_entry.kub_boost_flag = 1;
			}

			if (!user->give_item(give_item, give_params, &rank_entry))
			{
				presents.emplace_back(give_item);
				rank_entry.present_list.emplace_back(give_item);
			}
		}

		database::present_box::add_items(user->get_user_id(),
			database::present_box::present_flag_expire, 
			expire_date, presents, text_id);

		const auto present_count = database::present_box::get_present_count(user->get_user_id());
		for (auto i = 0ull; i < rank_reward_infos.size(); i++)
		{
			auto& info = rank_reward_infos[i];
			if (info.text_id != 0)
			{
				info.present_box_num = present_count;
				info.to_json(defense_mission_reward_j[defense_mission_reward_j.size()]);
			}
		}

		user->current_player->set_inventory_resource_list(resource_list);
		user->current_player->set_stackable_item_list(stackable_list);
		user->current_player->set_nonstackable_item_list(nonstackable_list);
		user->current_player->set_inventory(player_inventory);
		user->set_inventory(*user_inventory);
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
				next_wave++;
				if (wave_result.result == 1)
				{
					mission_result = 1u;
					this->generate_defense_mission_rewards(result, user, reward_list, *mission_settings, rank);
				}
				else
				{
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
				this->generate_defense_mission_rewards(result, user, reward_list, *mission_settings, rank);
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
		auto& replay_mission_info_j = data["replay_mission_info"];
		auto& quest_repop_count_decrement_j = data["quest_repop_count_decrement"];
		auto& play_record_save_checkpoint_j = data["play_record_save_checkpoint"];
		auto& play_record_additional_130_checkpoint_j = data["play_record_additional_130_checkpoint"];

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

		if ((tips_open_info_j.is_array() && tips_open_info_j.size()) || story_unlock_info_j.is_object())
		{
			const auto story_unlock_info = std::make_unique<database::players::story_unlock_info_t>();
			user->current_player->get_story_unlock_info(*story_unlock_info);

			story_unlock_info->parse(story_unlock_info_j);
			parse_map_unlock_list(story_unlock_info_j);

			if (tips_open_info_j.is_array() && tips_open_info_j.size())
			{
				story_unlock_info->tips_open_info.parse(tips_open_info_j[0]);
			}

			user->current_player->set_story_unlock_info(*story_unlock_info);
		}

		if (mission_info_j.is_object() || replay_mission_info_j.is_array())
		{
			const auto mission_info = std::make_unique<database::players::mission_info_t>();
			user->current_player->get_mission_info(*mission_info);

			if (mission_info_j.is_object() && !mission_info->parse(mission_info_j))
			{
				return error(ERR_INVALIDARG);
			}

			if (replay_mission_info_j.size() && !mission_info->replay_mission_info.parse(replay_mission_info_j[0]))
			{
				return error(ERR_INVALIDARG);
			}

			user->current_player->set_mission_info(*mission_info);
		}

		if (open_list_j.is_object() || quest_repop_count_decrement_j.is_number())
		{
			auto quest_record_list = user->current_player->get_quest_record_list();

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
					quest_record_list.parse_diff(quest_mission_code_list_j);
				}
			}

			const auto repop_dec = quest_repop_count_decrement_j.as<std::uint8_t>();
			for (auto i = 0ull; i < ARRAYSIZE(quest_record_list.list); i++)
			{
				quest_record_list.list[i].repop_count -= std::min(quest_record_list.list[i].repop_count, repop_dec);
			}

			user->current_player->set_quest_record_list(quest_record_list);
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

					database::crew_members::update(user->current_player->get_player_id(), update_params.unique_index, update_params);
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

		if (play_record_save_checkpoint_j.is_array() && play_record_additional_130_checkpoint_j.is_array())
		{
			const auto play_record = std::make_unique<database::players::player_play_record_t>();
			user->current_player->get_play_record(*play_record);
			play_record->parse_save(play_record_save_checkpoint_j, play_record_additional_130_checkpoint_j);
			user->current_player->set_play_record(*play_record);
		}

		database::challenge_tasks::send_progress_list(user->current_player->get_player_id(), data["order_progress_list"]);

		return result;
	}

	std::uint32_t cmd_checkpoint_save::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
