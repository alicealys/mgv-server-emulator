#include <std_include.hpp>

#include "cmd_checkpoint_save.hpp"

namespace emulator::ssd
{
	nlohmann::json cmd_checkpoint_save::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;

		const auto player_inventory = std::make_unique<database::players::player_inventory_t>();
		const auto loadout_list = std::make_unique<database::players::loadout_list_t>();
		const auto gimmick_info = std::make_unique<database::players::gimmick_info_t>();
		const auto mission_info = std::make_unique<database::players::mission_info_t>();

		auto& player_inventory_j = data["inventory_player_info_save"];
		auto& loadout_list_j = data["load_out_list"];
		auto& gimmick_timer_info_j = data["gimmick_timer_info"];
		auto& gimmick_save_info_j = data["gimmick_save_info"];
		auto& mission_info_j = data["current_mission_info"];

		if (!player_inventory_j.is_object() ||
			!loadout_list_j.is_array() || 
			!gimmick_timer_info_j.is_object() ||
			!gimmick_save_info_j.is_object() || 
			!mission_info_j.is_object())
		{
			return error(ERR_INVALIDARG);
		}

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

		user->current_player->get_mission_info(*mission_info);
		user->current_player->get_inventory(*player_inventory);
		user->current_player->get_loadout_list(*loadout_list);

		if (!mission_info->parse(mission_info_j))
		{
			return error(ERR_INVALIDARG);
		}

		const auto new_loadout = std::make_unique<database::players::loadout_t>();
		const auto loadout_count = std::min(static_cast<std::size_t>(user->current_player->get_loadout_count()), loadout_list_j.size());

		std::uint16_t nameplate{};
		player_inventory->parse_save(player_inventory_j, nameplate);

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

		// TODO
		// base_resource_count
		// group_level
		// open_list
		// play_record_additional_130_checkpoint
		// play_record_save_checkpoint
		// replay_mission_info
		// quest_repop_count_decrement
		// story_unlock_info
		// tips_open_info

		user->current_player->set_inventory(*player_inventory);
		user->current_player->set_loadout_list(*loadout_list);
		user->current_player->set_gimmick_info(*gimmick_info);
		user->current_player->set_mission_info(*mission_info);
		user->current_player->set_nameplate(nameplate);

		return result;
	}

	std::uint32_t cmd_checkpoint_save::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
