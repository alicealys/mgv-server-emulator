#include <std_include.hpp>

#include "cmd_checkpoint_save.hpp"

namespace emulator::ssd
{
	nlohmann::json cmd_checkpoint_save::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;

		const auto mission_info = std::make_unique<database::players::mission_info_t>();

		auto& player_inventory_j = data["inventory_player_info_save"];
		auto& user_inventory_j = data["inventory_user_info_save"];
		auto& loadout_list_j = data["load_out_list"];
		auto& gimmick_timer_info_j = data["gimmick_timer_info"];
		auto& gimmick_save_info_j = data["gimmick_save_info"];
		auto& mission_info_j = data["current_mission_info"];
		auto& base_resource_j = data["base_resource"];
		auto& base_resource_count_j = data["base_resource_count"];
		auto& animal_list_j = data["animal_list"];

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
			user->current_player->get_mission_info(*mission_info);

			if (!mission_info->parse(mission_info_j))
			{
				return error(ERR_INVALIDARG);
			}

			user->current_player->set_mission_info(*mission_info);
		}

		if (user_inventory_j.is_object())
		{
			const auto user_inventory = std::make_unique<database::users::user_inventory_t>();
			user->get_inventory(*user_inventory);
			user_inventory->parse_save(user_inventory_j);
			user->set_inventory(*user_inventory);
		}

		if (player_inventory_j.is_object())
		{
			const auto player_inventory = std::make_unique<database::players::player_inventory_t>();
			user->current_player->get_inventory(*player_inventory);

			std::uint16_t nameplate{};
			player_inventory->parse_save(player_inventory_j, nameplate);

			user->current_player->set_inventory(*player_inventory);
			user->current_player->set_nameplate(nameplate);
		}

		if (loadout_list_j.is_array())
		{
			const auto new_loadout = std::make_unique<database::players::loadout_t>();
			const auto loadout_count = std::min(static_cast<std::size_t>(user->current_player->get_loadout_count()), loadout_list_j.size());

			const auto loadout_list = std::make_unique<database::players::loadout_list_t>();
			user->current_player->get_loadout_list(*loadout_list);

			for (auto i = 0ull; i < loadout_count; i++)
			{
				std::memset(new_loadout.get(), 0, sizeof(database::players::loadout_t));
				std::uint32_t index{};
				if (!new_loadout->parse(loadout_list_j[i], index))
				{
					return error(ERR_INVALIDARG);
				}

				if (index >= user->current_player->get_loadout_count())
				{
					continue;
				}

				std::memcpy(&loadout_list->list[index], new_loadout.get(), sizeof(database::players::loadout_t));
			}

			user->current_player->set_loadout_list(*loadout_list);
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

		// TODO
		// group_level
		// open_list
		// play_record_additional_130_checkpoint
		// play_record_save_checkpoint
		// replay_mission_info
		// quest_repop_count_decrement
		// story_unlock_info
		// tips_open_info

		return result;
	}

	std::uint32_t cmd_checkpoint_save::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
