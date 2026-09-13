#include <std_include.hpp>

#include "cmd_checkpoint_save.hpp"
#include "cmd_inventory_save.hpp"

namespace emulator::ssd
{
	nlohmann::json cmd_checkpoint_save::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;

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
		auto& group_level_j = data["group_level"];

		result["added_crew"] = nlohmann::json::array();
		result["capture_list"] = nlohmann::json::array();
		result["defense_mission_reward"] = nlohmann::json::array();
		result["left_resources"] = nlohmann::json::array();
		result["stackable_list"] = nlohmann::json::array();
		result["reward_event_point"] = 0;
		result["boost_flag"] = 0;
		result["defense_reward_limit_result"]["limit_result"] = nlohmann::json::array();
		result["defense_reward_limit_result"]["mission_code"] = 0;

		if (gimmick_save_info_j.is_object())
		{
			auto& map_location_j = gimmick_save_info_j["map_location"];
			if (!map_location_j.is_number_integer())
			{
				return error(ERR_INVALIDARG);
			}

			const auto gimmick_info = std::make_unique<database::players::gimmick_info_t>();
			user->current_player->get_gimmick_info(*gimmick_info);

			const auto map_location = map_location_j.get<std::uint32_t>();
			if (map_location > 1)
			{
				return error(ERR_INVALIDARG);
			}

			switch (map_location)
			{
			case 0:
				gimmick_info->resource_afghan.parse(gimmick_timer_info_j);
				break;
			case 1:
				gimmick_info->resource_africa.parse(gimmick_timer_info_j);
				break;
			}

			if (gimmick_timer_info_j.is_object())
			{
				gimmick_info->timer.parse(gimmick_timer_info_j);
			}

			user->current_player->set_gimmick_info(*gimmick_info);

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
			//base_resources->parse_animals(animal_list_j[0]);
		}

		user->current_player->set_base_resources(*base_resources);

		const auto parse_map_unlock_list = [&](nlohmann::json& story_unlock_info_j)
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
				if (!location_index_j.is_number_unsigned() || !map_unlock_j.is_array() || map_unlock_j.size() == 0)
				{
					continue;
				}

				const auto location_index = location_index_j.get<std::uint32_t>();
				const auto map_unlock_list = std::make_unique<database::players::map_unlock_list_t>();

				switch (location_index)
				{
				case 0:
					user->current_player->get_map_unlock_list_afghan(*map_unlock_list, map_unlock_j.size());
					break;
				case 1:
					user->current_player->get_map_unlock_list_africa(*map_unlock_list, map_unlock_j.size());
					break;
				default:
					continue;
				}

				map_unlock_list->parse_diff(map_unlock_j);

				switch (location_index)
				{
				case 0:
					user->current_player->set_map_unlock_list_afghan(*map_unlock_list);
					break;
				case 1:
					user->current_player->set_map_unlock_list_africa(*map_unlock_list);
					break;
				}
			}
		};

		if (story_unlock_info_j.is_object())
		{
			const auto story_unlock_info = std::make_unique<database::players::story_unlock_info_t>();
			user->current_player->get_story_unlock_info(*story_unlock_info);
			story_unlock_info->parse(story_unlock_info_j);
			user->current_player->set_story_unlock_info(*story_unlock_info);
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
				const auto mission_record_list = std::make_unique<database::players::mission_record_list_t>();
				user->current_player->get_mission_record_list(*mission_record_list, mission_code_list_j.size());

				for (auto i = 0ull; i < mission_code_list_j.size(); i++)
				{
					auto& mission_code_j = mission_code_list_j[i];
					if (!mission_code_j.is_number_unsigned())
					{
						continue;
					}

					const auto mission_code = mission_code_j.get<std::uint32_t>();
					mission_record_list->open_mission(mission_code);
				}

				user->current_player->set_mission_record_list(*mission_record_list);
			}
		}

		if (reward_crew_j.is_array())
		{
			auto count = 0u;

			const auto crew_member_list = std::make_unique<database::players::crew_member_list_t>();
			user->current_player->get_crew_member_list(*crew_member_list, reward_crew_j.size());
			for (auto i = 0ull; i < reward_crew_j.size(); i++)
			{
				database::players::crew_member_t new_member{};
				if (!new_member.parse_add_param(reward_crew_j[i]))
				{
					continue;
				}

				const auto iter = game::parameters_table.ssd_crew_generator_table->crew_member_types.find(new_member.unique_index);
				if (iter == game::parameters_table.ssd_crew_generator_table->crew_member_types.end())
				{
					continue;
				}

				new_member.unique_id = utils::cryptography::random::get_integer() % 1000000;
				new_member.face_id = iter->second->face;
				new_member.race_id = iter->second->race;
				new_member.body_id = iter->second->body;
				new_member.ability_accessory = 0;
				new_member.ability_animal = 0;
				new_member.ability_base_defense = iter->second->base_defense;
				new_member.ability_defense_unit = 0;
				new_member.ability_develop = iter->second->develop;
				new_member.ability_expedition = iter->second->combat_deploy;
				new_member.ability_food = iter->second->food;
				new_member.ability_gadget = 0;
				new_member.ability_medical = iter->second->medic;
				new_member.ability_plant = iter->second->farm;
				new_member.resistance_food_shortage = iter->second->hunger_resist;
				new_member.resistance_sleepless = iter->second->sleeplack_resist;
				new_member.resistance_water_shortage = iter->second->thirst_resist;
				new_member.sanity = 1000;
				new_member.max_life = iter->second->life;
				new_member.life = iter->second->life;
				new_member.initial_max_life = iter->second->life;
				new_member.current_group = 1;
				new_member.previous_group = 1;

				crew_member_list->push(new_member);
				new_member.to_json(result["added_crew"][count++]);
			}

			user->current_player->set_crew_member_list(*crew_member_list);
		}

		if (crew_update_list_j.is_array())
		{
			auto crew_member_list = std::make_unique<database::players::crew_member_list_t>();
			user->current_player->get_crew_member_list(*crew_member_list);
			crew_member_list->parse_update(crew_update_list_j);
			user->current_player->set_crew_member_list(*crew_member_list);
		}

		if (group_level_j.is_object())
		{
			database::players::crew_levels_t levels{};
			levels.parse(group_level_j);
			user->current_player->set_crew_levels(levels);
		}

		// TODO
		// play_record_additional_130_checkpoint
		// play_record_save_checkpoint
		// replay_mission_info
		// quest_repop_count_decrement
		// tips_open_info

		return result;
	}

	std::uint32_t cmd_checkpoint_save::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
