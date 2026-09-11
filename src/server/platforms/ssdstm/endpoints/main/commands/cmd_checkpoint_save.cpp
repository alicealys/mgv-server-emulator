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

		auto story_sequence_number = -1;
		if (story_unlock_info_j.is_object())
		{
			const auto story_unlock_info = std::make_unique<database::players::story_unlock_info_t>();
			user->current_player->get_story_unlock_info(*story_unlock_info);
			story_unlock_info->parse(story_unlock_info_j);
			user->current_player->set_story_unlock_info(*story_unlock_info);
			story_sequence_number = story_unlock_info->story_sequence_number;
			parse_map_unlock_list(story_unlock_info_j);
		}

		if (mission_info_j.is_object())
		{
			const auto mission_info = std::make_unique<database::players::mission_info_t>();
			user->current_player->get_mission_info(*mission_info);

			if (story_sequence_number != -1)
			{
				mission_info->story_sequence_number = static_cast<std::uint16_t>(story_sequence_number);
			}

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

		// TODO
		// group_level
		// open_list
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
