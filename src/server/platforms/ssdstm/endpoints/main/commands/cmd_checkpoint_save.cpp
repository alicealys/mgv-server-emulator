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

		if (gimmick_timer_info_j.is_object() && gimmick_save_info_j.is_object())
		{
			const auto gimmick_info = std::make_unique<database::players::gimmick_info_t>();
			auto& map_location_j = gimmick_save_info_j["map_location"];
			if (!map_location_j.is_number_integer())
			{
				return error(ERR_INVALIDARG);
			}

			const auto map_location = map_location_j.get<std::uint32_t>();
			switch (map_location)
			{
			case 0:
				gimmick_info->resource_afghan.parse(gimmick_timer_info_j);
				break;
			case 1:
				gimmick_info->resource_africa.parse(gimmick_timer_info_j);
				break;
			default:
				return error(ERR_INVALIDARG);
			}

			gimmick_info->timer.parse(gimmick_timer_info_j);
			user->current_player->set_gimmick_info(*gimmick_info);
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

		if (story_unlock_info_j.is_object())
		{
			const auto story_unlock_info = std::make_unique<database::players::story_unlock_info_t>();
			user->current_player->get_story_unlock_info(*story_unlock_info);
			story_unlock_info->parse(story_unlock_info_j);
			user->current_player->set_story_unlock_info(*story_unlock_info);
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
