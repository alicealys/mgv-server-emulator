#include <std_include.hpp>

#include "cmd_mission_progress_load.hpp"

namespace emulator::ssd
{
	nlohmann::json cmd_mission_progress_load::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;

		static const auto default_data = utils::resources::load_json(RESOURCE_DEFAULT_DATA);

		const auto mission_info = std::make_unique<database::players::mission_info_t>();
		const auto gimmick_data_afghan = std::make_unique<database::players::gimmick_save_data_t>();
		const auto gimmick_data_africa = std::make_unique<database::players::gimmick_save_data_t>();
		const auto gimmick_info = std::make_unique<database::players::gimmick_info_t>();
		const auto player_inventory = std::make_unique<database::players::player_inventory_t>();
		const auto nonstackable_list = std::make_unique<database::players::nonstackable_list_t>();
		const auto loadout_list = std::make_unique<database::players::loadout_list_t>();
		const auto user_inventory = std::make_unique<database::users::user_inventory_t>();
		const auto user_play_record = std::make_unique<database::users::user_play_record_t>();
		const auto player_play_record = std::make_unique<database::players::player_play_record_t>();

		const auto& player = user->current_player;

		player->get_mission_info(*mission_info);
		player->get_gimmick_info(*gimmick_info);
		player->get_gimmick_save_data(*gimmick_data_afghan, 0);
		player->get_gimmick_save_data(*gimmick_data_africa, 1);
		player->get_inventory(*player_inventory);
		player->get_loadout_list(*loadout_list);
		player->get_play_record(*player_play_record);
		player->get_nonstackable_list(*nonstackable_list);

		user->get_inventory(*user_inventory);
		user->get_play_record(*user_play_record);

		result["additional_storage_info"]["count"] = {0, 0};
		result["battle_pack_list"] = nlohmann::json::array();
		result["bgm_my_list"] = nlohmann::json::array();
		result["boost_list"] = nlohmann::json::array();
		result["bp_mission_list"] = default_data["bp_mission_list"];
		result["cage_list"] = nlohmann::json::array();
		result["communication_gesture_slot"] = default_data["communication_gesture_slot"];

		result["coop_embedded_mission_record_info_list"] = nlohmann::json::array();
		result["coop_event_mission_record_info_list"] = nlohmann::json::array();
		result["coop_reward_limit_list"] = nlohmann::json::array();

		result["craft_board"] = nlohmann::json::array();

		mission_info->to_json(result["current_mission_info"]);

		result["defense_mission_parameter"]["attack_time"] = 0;
		result["defense_mission_parameter"]["flag"] = 0;
		result["defense_mission_parameter"]["threat"] = 0;
		result["defense_mission_parameter"]["threat_threshold"] = 0;
		result["defense_mission_record_info_list"] = nlohmann::json::array();
		result["defense_mission_status"]["disconnect_flag"] = 0;
		result["defense_mission_status"]["mining_machine_life"] = 9680;
		result["defense_mission_status"]["mission_code"] = 0;
		result["defense_mission_status"]["mission_start_date"] = 0;
		result["defense_mission_status"]["next_wave_start_date"] = 0;
		result["defense_mission_status"]["total_score"] = 0;
		result["defense_mission_status"]["wave"] = 0;
		result["defense_reward_limit_list"] = nlohmann::json::array();

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

		result["mission_record_info_list"] = nlohmann::json::array();
		nonstackable_list->to_json(result["nonstackable_list"]);

		result["order_expired_list"] = {0, 0, 0, 0, 0};
		result["order_task_list"] = nlohmann::json::array();

		for (auto i = 0; i < ARRAYSIZE(player_play_record->first); i++)
		{
			result["play_record_player"][i] = player_play_record->first[i];
			result["play_record_user_total"][i] = user_play_record->first[i];
		}

		for (auto i = 0; i < ARRAYSIZE(player_play_record->additional); i++)
		{
			result["play_record_player_additional_130"][i] = player_play_record->additional[i];
			result["play_record_user_total_additional_130"][i] = user_play_record->additional[i];
		}

		result["quest_record_info_list"] = nlohmann::json::array();
		result["recipe_list"] = nlohmann::json::array();

		for (auto i = 0; i < 256; i++)
		{
			result["recommend_bp_mission_list"][i] = 0;
		}

		result["replay_info_list"] = nlohmann::json::array();
		result["replay_mission_info"]["is_replay_mission"] = 0;
		result["replay_mission_info"]["replay_mission_difficalty"] = 0;
		result["replay_mission_info"]["replay_mission_id"] = 0;
		result["replay_mission_info"]["replay_mission_return_location_code"] = 0;
		result["replay_mission_info"]["replay_mission_return_mission_code"] = 0;

		result["resource_list"] = nlohmann::json::array();
		result["stackable_list"] = nlohmann::json::array();

		result["story_unlock_info"]["demo_open_flag"] = 0;
		result["story_unlock_info"]["facility_new_flag"] = 0;
		result["story_unlock_info"]["fast_travel_unlock"] = "";
		result["story_unlock_info"]["map_unlock_list"][0]["location_index"] = 1;
		result["story_unlock_info"]["map_unlock_list"][0]["map_unlock"][0] = 0;
		result["story_unlock_info"]["marker_afghan"] = "";
		result["story_unlock_info"]["marker_africa"] = "";
		result["story_unlock_info"]["marker_map_location"] = 0;
		result["story_unlock_info"]["oxygen_supply_unlock"] = 0;
		result["story_unlock_info"]["story_sequence_number"] = 0;
		
		result["tips_open_info"]["data"] = "";

		result["user_flag"] = user->get_user_flag();

        return result;
	}

	std::uint32_t cmd_mission_progress_load::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
