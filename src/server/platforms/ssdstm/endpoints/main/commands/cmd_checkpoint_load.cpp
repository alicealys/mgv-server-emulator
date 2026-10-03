#include <std_include.hpp>

#include "cmd_checkpoint_load.hpp"

namespace emulator::ssd
{
	json::value cmd_checkpoint_load::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		const auto mission_info = std::make_unique<database::players::mission_info_t>();
		const auto gimmick_data_afghan = std::make_unique<database::players::gimmick_save_data_t>();
		const auto gimmick_data_africa = std::make_unique<database::players::gimmick_save_data_t>();
		const auto player_inventory = std::make_unique<database::players::player_inventory_t>();
		const auto nonstackable_list = std::make_unique<database::players::nonstackable_item_list_t>();
		const auto loadout_list = std::make_unique<database::players::loadout_list_t>();
		const auto user_inventory = std::make_unique<database::users::user_inventory_t>();
		const auto story_unlock_info = std::make_unique<database::players::story_unlock_info_t>();
		const auto inventory_resources = std::make_unique<database::players::inventory_resource_list_t>();
		const auto stackable_item_list = std::make_unique<database::players::stackable_item_list_t>();
		const auto map_unlock_list_afghan = std::make_unique<database::players::map_unlock_list_t>();
		const auto map_unlock_list_africa = std::make_unique<database::players::map_unlock_list_t>();

		const auto& player = user->current_player;

		player->get_mission_info(*mission_info);
		player->get_gimmick_save_data(*gimmick_data_afghan, 0);
		player->get_gimmick_save_data(*gimmick_data_africa, 1);
		player->get_inventory(*player_inventory);
		player->get_loadout_list(*loadout_list);
		player->get_nonstackable_item_list(*nonstackable_list);
		player->get_inventory_resource_list(*inventory_resources);
		player->get_stackable_item_list(*stackable_item_list);
		player->get_story_unlock_info(*story_unlock_info);
		player->get_map_unlock_list_afghan(*map_unlock_list_afghan);
		player->get_map_unlock_list_africa(*map_unlock_list_africa);

		const auto gimmick_info = player->get_gimmick_info();

		user->get_inventory(*user_inventory);

		result["craft_board"] = json::array();

		mission_info->to_json(result["current_mission_info"]);

		result["gimmick_save_info_afghan"]["map_location"] = 0;
		gimmick_data_afghan->to_json(result["gimmick_save_info_afghan"], 0);
		gimmick_info.resource_afghan.to_json(result["gimmick_save_info_afghan"]);

		result["gimmick_save_info_africa"]["map_location"] = 1;
		gimmick_data_africa->to_json(result["gimmick_save_info_africa"], 1);
		gimmick_info.resource_africa.to_json(result["gimmick_save_info_afghan"]);

		gimmick_info.timer.to_json(result["gimmick_timer_info"]);

		player_inventory->to_json(result["inventory_player_info"], static_cast<std::uint16_t>(player->get_nameplate()));
		user_inventory->to_json(result["inventory_user_info"]);

		auto& loadout_list_j = result["load_out_list"];
		for (auto i = 0u; i < user->get_loadout_count(); i++)
		{
			loadout_list->list[i].to_json(loadout_list_j[i], static_cast<std::uint16_t>(i));
		}

		result["story_unlock_info"]["map_unlock_list"][0]["location_index"] = 0;
		map_unlock_list_afghan->to_json(result["story_unlock_info"]["map_unlock_list"][0]["map_unlock"]);

		result["story_unlock_info"]["map_unlock_list"][1]["location_index"] = 1;
		map_unlock_list_africa->to_json(result["story_unlock_info"]["map_unlock_list"][1]["map_unlock"]);

		story_unlock_info->to_json(result["story_unlock_info"]);

		nonstackable_list->to_json(result["nonstackable_list"]);
		inventory_resources->to_json(result["resource_list"]);
		stackable_item_list->to_json(result["stackable_list"]);
		result["recipe_list"] = json::array();

		return result;
	}

	std::uint32_t cmd_checkpoint_load::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
