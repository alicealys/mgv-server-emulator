#include <std_include.hpp>

#include "cmd_mission_progress_load.hpp"

#include "database/models/defense_missions.hpp"
#include "database/models/shop_purchases.hpp"
#include "database/models/challenge_tasks.hpp"
#include "database/models/coop_missions.hpp"
#include "database/models/boosts.hpp"
#include "database/models/events.hpp"
#include "database/models/crew_members.hpp"

namespace emulator::ssd
{
	json::value cmd_mission_progress_load::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		const auto& player = user->current_player;

		result["cage_list"] = json::array();
		result["craft_board"] = json::array();
		result["recipe_list"] = json::array();

		result["user_flag"] = user->get_user_flag();

		database::challenge_tasks::dump_order_list(user->current_player->get_player_id(),
			result["order_expired_list"], result["order_task_list"]);

		{
			auto& additional_storage_info_j = result["additional_storage_info"];
			auto& count_j = additional_storage_info_j["count"];
			count_j[0] = user->get_additional_storage_01();
			count_j[1] = user->get_additional_storage_02();
		}

		{
			const auto communication_gesture_info = player->get_communication_gesture_info();
			communication_gesture_info.to_json(result["communication_gesture_slot"]);
		}

		{
			auto& boost_list_j = result["boost_list"];
			boost_list_j = json::array();

			auto count = 0u;
			const auto boost_list = database::boosts::get_boost_list(user->get_user_id());
			for (auto i = 0ull; i < boost_list.size(); i++)
			{
				if (!boost_list[i].valid)
				{
					continue;
				}

				boost_list[i].to_json(boost_list_j[count++]);
			}
		}

		{
			auto& bp_mission_list_j = result["bp_mission_list"];
			auto& recommend_bp_mission_list_j = result["recommend_bp_mission_list"];

			recommend_bp_mission_list_j = json::array();

			database::events::cached_event.access([&](const std::optional<database::events::running_event>& event)
			{
				if (!event.has_value())
				{
					bp_mission_list_j = json::array();
					return;
				}

				bp_mission_list_j = event->params->bp_mission_list;
			});
		}

		{
			auto& coop_embedded_mission_record_info_list_j = result["coop_embedded_mission_record_info_list"];
			auto& coop_event_mission_record_info_list_j = result["coop_event_mission_record_info_list"];
			auto& coop_reward_limit_list_j = result["coop_reward_limit_list"];

			coop_embedded_mission_record_info_list_j = json::array();
			coop_event_mission_record_info_list_j = json::array();
			coop_reward_limit_list_j = json::array();

			const auto embedded_list = database::coop_missions::get_record_list(user->current_player->get_player_id(), database::coop_missions::mission_type_embedded);
			const auto event_list = database::coop_missions::get_record_list(user->current_player->get_player_id(), database::coop_missions::mission_type_event);

			for (auto i = 0ull; i < embedded_list.size(); i++)
			{
				embedded_list[i].to_json(coop_embedded_mission_record_info_list_j[i]);
			}

			for (auto i = 0ull; i < event_list.size(); i++)
			{
				event_list[i].to_json(coop_event_mission_record_info_list_j);
			}
		}

		{
			const auto mission_info = std::make_unique<database::players::mission_info_t>();
			player->get_mission_info(*mission_info);
			mission_info->to_json(result["current_mission_info"]);
			mission_info->replay_mission_info.to_json(result["replay_mission_info"]);
		}

		{
			const auto quest_record_list = player->get_quest_record_list();
			const auto mission_record_list = player->get_mission_record_list();
			const auto replay_info_list = player->get_replay_info_list();

			const auto crew_size = database::crew_members::get_member_count(user->current_player->get_player_id());
			const auto skip_crew_quests = crew_size >= database::crew_members::max_crew_members;

			mission_record_list.to_json(result["mission_record_info_list"]);
			quest_record_list.to_json(result["quest_record_info_list"], skip_crew_quests);
			replay_info_list.to_json(result["replay_info_list"]);
		}

		{
			auto& defense_mission_status_j = result["defense_mission_status"];
			auto& defense_mission_record_info_list_j = result["defense_mission_record_info_list"];

			const auto defense_mission_info = player->get_defense_mission_info();
			const auto latest_defense_mission = database::defense_missions::get_current_mission(user->current_player->get_player_id());

			if (latest_defense_mission.has_value())
			{
				defense_mission_status_j["mission_code"] = latest_defense_mission->get_mission_code();
				defense_mission_status_j["wave"] = latest_defense_mission->get_current_wave();
				defense_mission_status_j["mission_start_date"] = latest_defense_mission->get_start_date();
				defense_mission_status_j["next_wave_start_date"] = latest_defense_mission->get_next_wave_date();
				defense_mission_status_j["total_score"] = latest_defense_mission->get_total_score();
				defense_mission_status_j["mining_machine_life"] = defense_mission_info.status.mining_machine_life;
				defense_mission_status_j["disconnect_flag"] = 0;
			}
			else
			{
				defense_mission_status_j["mission_code"] = 0;
				defense_mission_status_j["wave"] = 0;
				defense_mission_status_j["mission_start_date"] = 0;
				defense_mission_status_j["next_wave_start_date"] = 0;
				defense_mission_status_j["total_score"] = 0;
				defense_mission_status_j["mining_machine_life"] = defense_mission_info.status.mining_machine_life;
				defense_mission_status_j["disconnect_flag"] = 0;
			}

			defense_mission_info.parameter.to_json(result["defense_mission_parameter"]);

			auto count = 0u;
			const auto defense_mission_record_list = database::defense_missions::get_record_list(user->current_player->get_player_id());
			for (auto i = 0ull; i < defense_mission_record_list.size(); i++)
			{
				if (defense_mission_record_list[i].cleared == 0)
				{
					continue;
				}

				auto& entry = defense_mission_record_info_list_j[count++];
				defense_mission_record_list[i].to_json(entry);
			}

			result["defense_reward_limit_list"] = json::array{};
		}

		{
			const auto gimmick_info = player->get_gimmick_info();
			const auto gimmick_datas = std::make_unique<database::players::gimmick_save_data_t[]>(2);
			auto& gimmick_data_afghan = gimmick_datas[0];
			auto& gimmick_data_africa = gimmick_datas[1];

			player->get_gimmick_save_data(gimmick_data_afghan, 0);
			player->get_gimmick_save_data(gimmick_data_africa, 1);

			result["gimmick_save_info_afghan"]["map_location"] = 0;
			gimmick_data_afghan.to_json(result["gimmick_save_info_afghan"], 0);
			gimmick_info.resource_afghan.to_json(result["gimmick_save_info_afghan"]);

			result["gimmick_save_info_africa"]["map_location"] = 1;
			gimmick_data_africa.to_json(result["gimmick_save_info_africa"], 1);
			gimmick_info.resource_africa.to_json(result["gimmick_save_info_afghan"]);

			gimmick_info.timer.to_json(result["gimmick_timer_info"]);
		}

		{
			const auto user_inventory = std::make_unique<database::users::user_inventory_t>();
			const auto player_inventory = std::make_unique<database::players::player_inventory_t>();

			player->get_inventory(*player_inventory);
			user->get_inventory(*user_inventory);

			player_inventory->to_json(result["inventory_player_info"], static_cast<std::uint16_t>(player->get_nameplate()));
			user_inventory->to_json(result["inventory_user_info"]);
			user_inventory->bgm_settings.to_json(result["bgm_my_list"]);
		}

		{
			const auto loadout_list = std::make_unique<database::players::loadout_list_t>();
			player->get_loadout_list(*loadout_list);

			auto& loadout_list_j = result["load_out_list"];
			for (auto i = 0u; i < user->get_loadout_count(); i++)
			{
				loadout_list->list[i].to_json(loadout_list_j[i], static_cast<std::uint16_t>(i));
			}
		}

		{
			database::players::nonstackable_item_list_t nonstackable_list;
			database::players::stackable_item_list_t stackable_item_list;
			database::players::inventory_resource_list_t resource_list;
			database::players::battle_pack_list_t battle_pack_list;

			player->get_nonstackable_item_list(nonstackable_list);
			player->get_inventory_resource_list(resource_list);
			player->get_stackable_item_list(stackable_item_list);
			player->get_battle_pack_list(battle_pack_list);

			nonstackable_list.to_json(result["nonstackable_list"]);
			battle_pack_list.to_json(result["battle_pack_list"]);
			resource_list.to_json(result["resource_list"]);
			stackable_item_list.to_json(result["stackable_list"]);
		}

		{
			const auto players = database::players::get_player_list(user->get_user_id());
			const auto play_records = std::make_unique<database::players::player_play_record_t[]>(2);
			auto& record_player = play_records[0];
			auto& record_total = play_records[1];

			player->get_play_record(record_player);

			for (const auto& other_player : players)
			{
				const auto play_record = std::make_unique<database::players::player_play_record_t>();
				other_player.get_play_record(*play_record);

				for (auto i = 0ull; i < ARRAYSIZE(play_record->first); i++)
				{
					record_total.first[i] += play_record->first[i];
				}

				for (auto i = 0ull; i < ARRAYSIZE(play_record->additional); i++)
				{
					record_total.additional[i] += play_record->additional[i];
				}
			}

			auto& play_record_player_j = result["play_record_player"];
			auto& play_record_user_total_j = result["play_record_user_total"];
			auto& play_record_player_additional_130_j = result["play_record_player_additional_130"];
			auto& play_record_user_total_additional_130_j = result["play_record_user_total_additional_130"];

			for (auto i = 0ull; i < ARRAYSIZE(record_player.first); i++)
			{
				play_record_player_j[i] = record_player.first[i];
				play_record_user_total_j[i] = record_total.first[i];
			}

			for (auto i = 0ull; i < ARRAYSIZE(record_player.additional); i++)
			{
				play_record_player_additional_130_j[i] = record_player.additional[i];
				play_record_user_total_additional_130_j[i] = record_total.additional[i];
			}
		}

		{
			auto& story_unlock_info_j = result["story_unlock_info"];
			auto& map_unlock_list_j = story_unlock_info_j["map_unlock_list"];

			const auto map_unlock_lists = std::make_unique<database::players::map_unlock_list_t[]>(2);
			const auto map_unlock_list_africa = std::make_unique<database::players::map_unlock_list_t>();
			const auto story_unlock_info = std::make_unique<database::players::story_unlock_info_t>();

			player->get_story_unlock_info(*story_unlock_info);
			player->get_map_unlock_list_afghan(map_unlock_lists[0]);
			player->get_map_unlock_list_africa(map_unlock_lists[1]);

			story_unlock_info->to_json(story_unlock_info_j);
			story_unlock_info->tips_open_info.to_json(result["tips_open_info"]);

			map_unlock_list_j[0]["location_index"] = 0;
			map_unlock_lists[0].to_json(map_unlock_list_j[0]["map_unlock"]);

			map_unlock_list_j[1]["location_index"] = 1;
			map_unlock_lists[1].to_json(map_unlock_list_j[1]["map_unlock"]);
		}
		
		return result;
	}

	std::uint32_t cmd_mission_progress_load::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
