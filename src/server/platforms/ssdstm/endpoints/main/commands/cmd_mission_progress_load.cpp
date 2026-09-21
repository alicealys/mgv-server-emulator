#include <std_include.hpp>

#include "cmd_mission_progress_load.hpp"

#include "database/models/defense_missions.hpp"

namespace emulator::ssd
{
	json::value cmd_mission_progress_load::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		static const auto default_data = utils::resources::load_json(RESOURCE_DEFAULT_DATA);

		const auto mission_info = std::make_unique<database::players::mission_info_t>();
		const auto gimmick_data_afghan = std::make_unique<database::players::gimmick_save_data_t>();
		const auto gimmick_data_africa = std::make_unique<database::players::gimmick_save_data_t>();
		const auto gimmick_info = std::make_unique<database::players::gimmick_info_t>();
		const auto player_inventory = std::make_unique<database::players::player_inventory_t>();
		const auto nonstackable_list = std::make_unique<database::players::nonstackable_item_list_t>();
		const auto loadout_list = std::make_unique<database::players::loadout_list_t>();
		const auto user_inventory = std::make_unique<database::users::user_inventory_t>();
		const auto user_play_record = std::make_unique<database::users::user_play_record_t>();
		const auto player_play_record = std::make_unique<database::players::player_play_record_t>();
		const auto story_unlock_info = std::make_unique<database::players::story_unlock_info_t>();
		const auto mission_record_list = std::make_unique<database::players::mission_record_list_t>();
		const auto quest_record_list = std::make_unique<database::players::quest_record_list_t>();
		const auto inventory_resources = std::make_unique<database::players::inventory_resource_list_t>();
		const auto stackable_item_list = std::make_unique<database::players::stackable_item_list_t>();
		const auto map_unlock_list_afghan = std::make_unique<database::players::map_unlock_list_t>();
		const auto map_unlock_list_africa = std::make_unique<database::players::map_unlock_list_t>();
		const auto defense_mission_info = std::make_unique<database::players::defense_mission_info_t>();
		const auto defense_mission_record_list = std::make_unique<database::players::defense_mission_record_list_t>();

		const auto& player = user->current_player;

		player->get_mission_info(*mission_info);
		player->get_gimmick_info(*gimmick_info);
		player->get_gimmick_save_data(*gimmick_data_afghan, 0);
		player->get_gimmick_save_data(*gimmick_data_africa, 1);
		player->get_inventory(*player_inventory);
		player->get_loadout_list(*loadout_list);
		player->get_play_record(*player_play_record);
		player->get_nonstackable_item_list(*nonstackable_list);
		player->get_inventory_resource_list(*inventory_resources);
		player->get_stackable_item_list(*stackable_item_list);
		player->get_story_unlock_info(*story_unlock_info);
		player->get_mission_record_list(*mission_record_list);
		player->get_quest_record_list(*quest_record_list);
		player->get_map_unlock_list_afghan(*map_unlock_list_afghan);
		player->get_map_unlock_list_africa(*map_unlock_list_africa);
		player->get_defense_mission_info(*defense_mission_info);
		player->get_defense_mission_record_list(*defense_mission_record_list);

		user->get_inventory(*user_inventory);
		user->get_play_record(*user_play_record);

		result["additional_storage_info"]["count"] = json::array{0, 0};
		result["battle_pack_list"] = json::array();
		result["bgm_my_list"] = json::array();
		result["boost_list"] = json::array();
		result["bp_mission_list"] = default_data["bp_mission_list"];
		result["cage_list"] = json::array();
		result["communication_gesture_slot"] = default_data["communication_gesture_slot"];

		result["coop_embedded_mission_record_info_list"] = json::array();
		result["coop_event_mission_record_info_list"] = json::array();
		result["coop_reward_limit_list"] = json::array();

		result["craft_board"] = json::array();

		mission_info->to_json(result["current_mission_info"]);

		const auto latest_defense_mission = database::defense_missions::get_current_mission(user->current_player->get_player_id());
		if (latest_defense_mission.has_value())
		{
			result["defense_mission_status"]["mission_code"] = latest_defense_mission->get_mission_code();
			result["defense_mission_status"]["wave"] = latest_defense_mission->get_current_wave();
			result["defense_mission_status"]["mission_start_date"] = latest_defense_mission->get_start_date();
			result["defense_mission_status"]["next_wave_start_date"] = latest_defense_mission->get_next_wave_date();
			result["defense_mission_status"]["total_score"] = latest_defense_mission->get_total_score();
			result["defense_mission_status"]["mining_machine_life"] = defense_mission_info->status.mining_machine_life;
			result["defense_mission_status"]["disconnect_flag"] = 0;
		}
		else
		{
			result["defense_mission_status"]["mission_code"] = 0;
			result["defense_mission_status"]["wave"] = 0;
			result["defense_mission_status"]["mission_start_date"] = 0;
			result["defense_mission_status"]["next_wave_start_date"] = 0;
			result["defense_mission_status"]["total_score"] = 0;
			result["defense_mission_status"]["mining_machine_life"] = defense_mission_info->status.mining_machine_life;
			result["defense_mission_status"]["disconnect_flag"] = 0;
		}

		defense_mission_info->parameter.to_json(result["defense_mission_parameter"]);
		defense_mission_record_list->to_json(result["defense_mission_record_info_list"]);

		result["defense_reward_limit_list"] = json::array{};

		result["gimmick_save_info_afghan"]["map_location"] = 0;
		gimmick_data_afghan->to_json(result["gimmick_save_info_afghan"], 0);
		gimmick_info->resource_afghan.to_json(result["gimmick_save_info_afghan"]);

		result["gimmick_save_info_africa"]["map_location"] = 1;
		gimmick_data_africa->to_json(result["gimmick_save_info_africa"], 1);
		gimmick_info->resource_africa.to_json(result["gimmick_save_info_afghan"]);

		gimmick_info->timer.to_json(result["gimmick_timer_info"]);

		player_inventory->to_json(result["inventory_player_info"]);
		user_inventory->to_json(result["inventory_user_info"]);

		auto& loadout_list_j = result["load_out_list"];
		for (auto i = 0u; i < player->get_loadout_count(); i++)
		{
			loadout_list->list[i].to_json(loadout_list_j[i], static_cast<std::uint16_t>(i));
		}

		mission_record_list->to_json(result["mission_record_info_list"]);
		nonstackable_list->to_json(result["nonstackable_list"]);

		result["order_expired_list"] = json::array{0, 0, 0, 0, 0};
		result["order_task_list"] = json::array();

		for (auto i = 0ull; i < ARRAYSIZE(player_play_record->first); i++)
		{
			result["play_record_player"][i] = player_play_record->first[i];
			result["play_record_user_total"][i] = user_play_record->first[i];
		}

		for (auto i = 0ull; i < ARRAYSIZE(player_play_record->additional); i++)
		{
			result["play_record_player_additional_130"][i] = player_play_record->additional[i];
			result["play_record_user_total_additional_130"][i] = user_play_record->additional[i];
		}

		quest_record_list->to_json(result["quest_record_info_list"]);
		result["recipe_list"] = json::array();

		for (auto i = 0; i < 256; i++)
		{
			result["recommend_bp_mission_list"][i] = 0;
		}

		result["replay_info_list"] = json::array();
		result["replay_mission_info"]["is_replay_mission"] = 0;
		result["replay_mission_info"]["replay_mission_difficalty"] = 0;
		result["replay_mission_info"]["replay_mission_id"] = 0;
		result["replay_mission_info"]["replay_mission_return_location_code"] = 0;
		result["replay_mission_info"]["replay_mission_return_mission_code"] = 0;

		inventory_resources->to_json(result["resource_list"]);
		stackable_item_list->to_json(result["stackable_list"]);
		story_unlock_info->to_json(result["story_unlock_info"]);
		story_unlock_info->tips_open_info.to_json(result["tips_open_info"]);

		result["story_unlock_info"]["map_unlock_list"][0]["location_index"] = 0;
		map_unlock_list_afghan->to_json(result["story_unlock_info"]["map_unlock_list"][0]["map_unlock"]);

		result["story_unlock_info"]["map_unlock_list"][1]["location_index"] = 1;
		map_unlock_list_africa->to_json(result["story_unlock_info"]["map_unlock_list"][1]["map_unlock"]);
		
		result["user_flag"] = user->get_user_flag();

        return result;
	}

	std::uint32_t cmd_mission_progress_load::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
