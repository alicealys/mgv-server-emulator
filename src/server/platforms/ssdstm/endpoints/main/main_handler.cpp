#include <std_include.hpp>

#include "main_handler.hpp"

#include "commands/cmd_auth_steamticket.hpp"
#include "commands/cmd_reqauth_https.hpp"
#include "commands/cmd_send_ipandport.hpp"
#include "commands/cmd_get_playerlist.hpp"
#include "commands/cmd_set_currentplayer.hpp"
#include "commands/cmd_create_player.hpp"
#include "commands/cmd_gdpr_check.hpp"
#include "commands/cmd_reqauth_sessionsvr.hpp"
#include "commands/cmd_send_heartbeat.hpp"
#include "commands/cmd_error_info_send.hpp"
#include "commands/cmd_error_lang_id_send.hpp"
#include "commands/cmd_get_informationlist.hpp"
#include "commands/cmd_update_session.hpp"
#include "commands/cmd_svcoin_get_balance.hpp"
#include "commands/cmd_check_consume_transaction.hpp"
#include "commands/cmd_start_consume_transaction.hpp"
#include "commands/cmd_commit_consume_transaction.hpp"
#include "commands/cmd_steam_shop_open.hpp"
#include "commands/cmd_steam_shop_approve.hpp"
#include "commands/cmd_steam_shop_get_item_list.hpp"
#include "commands/cmd_purchase_player_slot.hpp"
#include "commands/cmd_purchase_deploy_team.hpp"
#include "commands/cmd_purchase_gesture.hpp"
#include "commands/cmd_purchase_marker.hpp"
#include "commands/cmd_purchase_radio.hpp"
#include "commands/cmd_purchase_load_out.hpp"
#include "commands/cmd_purchase_calc_deploy_reduction_price.hpp"
#include "commands/cmd_purchase_deploy_reduction.hpp"
#include "commands/cmd_purchase_additional_storage.hpp"
#include "commands/cmd_purchase_calc_defense_mission_reduction_price.hpp"
#include "commands/cmd_purchase_defense_mission_reduction.hpp"
#include "commands/cmd_purchase_get_product_list.hpp"
#include "commands/cmd_purchase_shop_get_item_list.hpp"
#include "commands/cmd_purchase_shop_buy_item.hpp"
#include "commands/cmd_purchase_get_history.hpp"
#include "commands/cmd_boost_add_confirm.hpp"
#include "commands/cmd_coop_mission_result.hpp"
#include "commands/cmd_matching_createjoin_room.hpp"
#include "commands/cmd_matching_leave_room.hpp"
#include "commands/cmd_matching_search_room.hpp"
#include "commands/cmd_matching_join_room.hpp"
#include "commands/cmd_matching_get_room_data.hpp"
#include "commands/cmd_matching_get_roommember_data.hpp"
#include "commands/cmd_matching_set_room_data_external.hpp"
#include "commands/cmd_matching_set_room_data_internal.hpp"
#include "commands/cmd_matching_set_roommember_data.hpp"
#include "commands/cmd_matching_set_room_owner.hpp"
#include "commands/cmd_matching_get_matching_player_num.hpp"
#include "commands/cmd_matching_kick_member.hpp"
#include "commands/cmd_matching_reserve_slot.hpp"
#include "commands/cmd_matching_migration_heartbeat.hpp"
#include "commands/cmd_matching_migration_get_roommember_data.hpp"
#include "commands/cmd_matching_alive_notice.hpp"
#include "commands/cmd_matching_alive_fix.hpp"
#include "commands/cmd_matching_get_lock_set_room_owner.hpp"
#include "commands/cmd_coop_item_use.hpp"
#include "commands/cmd_coop_mission_end_confirm.hpp"
#include "commands/cmd_coop_mission_start.hpp"
#include "commands/cmd_coop_mission_end.hpp"
#include "commands/cmd_coop_flag_set.hpp"
#include "commands/cmd_coop_flag_reset.hpp"
#include "commands/cmd_craft_item.hpp"
#include "commands/cmd_get_craft_param.hpp"
#include "commands/cmd_checkpoint_save.hpp"
#include "commands/cmd_checkpoint_load.hpp"
#include "commands/cmd_avatar_save.hpp"
#include "commands/cmd_avatar_load.hpp"
#include "commands/cmd_mission_start.hpp"
#include "commands/cmd_mission_end.hpp"
#include "commands/cmd_mission_replay_end.hpp"
#include "commands/cmd_flag_mission_start.hpp"
#include "commands/cmd_flag_mission_end.hpp"
#include "commands/cmd_mission_abort.hpp"
#include "commands/cmd_quest_end.hpp"
#include "commands/cmd_mission_open.hpp"
#include "commands/cmd_mission_progress_load.hpp"
#include "commands/cmd_event_log_send.hpp"
#include "commands/cmd_character_set_name.hpp"
#include "commands/cmd_character_set_class.hpp"
#include "commands/cmd_server_data_initialize.hpp"
#include "commands/cmd_survivor_initialize.hpp"
#include "commands/cmd_mission_defense_mission_start.hpp"
#include "commands/cmd_dlc_set_flag.hpp"
#include "commands/cmd_dlc_get_flag.hpp"
#include "commands/cmd_communication_gesture_slot_save.hpp"
#include "commands/cmd_cheat_send.hpp"
#include "commands/cmd_base_resource_provide.hpp"
#include "commands/cmd_base_resource_load.hpp"
#include "commands/cmd_base_resource_consume.hpp"
#include "commands/cmd_base_resource_income.hpp"
#include "commands/cmd_base_resource_draw_water.hpp"
#include "commands/cmd_base_resource_update.hpp"
#include "commands/cmd_building_create.hpp"
#include "commands/cmd_building_load.hpp"
#include "commands/cmd_building_relocate.hpp"
#include "commands/cmd_building_remove.hpp"
#include "commands/cmd_building_damage.hpp"
#include "commands/cmd_building_repair.hpp"
#include "commands/cmd_building_repair_list.hpp"
#include "commands/cmd_building_mining_machine_repair.hpp"
#include "commands/cmd_deploy_get_list.hpp"
#include "commands/cmd_crew_load.hpp"
#include "commands/cmd_crew_group_transfer.hpp"
#include "commands/cmd_crew_banish.hpp"
#include "commands/cmd_crew_add.hpp"
#include "commands/cmd_crew_update.hpp"
#include "commands/cmd_crew_treat.hpp"
#include "commands/cmd_crew_use_item.hpp"
#include "commands/cmd_crew_use_party_item.hpp"
#include "commands/cmd_deploy_load_team.hpp"
#include "commands/cmd_deploy_save_team.hpp"
#include "commands/cmd_deploy_deploy.hpp"
#include "commands/cmd_deploy_complete.hpp"
#include "commands/cmd_deploy_supply_item.hpp"
#include "commands/cmd_server_parameter_load.hpp"
#include "commands/cmd_server_parameter_get_url_list.hpp"
#include "commands/cmd_information_get_title.hpp"
#include "commands/cmd_information_get_text.hpp"
#include "commands/cmd_information_get_detail.hpp"
#include "commands/cmd_present_box_get_list.hpp"
#include "commands/cmd_present_box_get_item.hpp"
#include "commands/cmd_present_box_get_all.hpp"
#include "commands/cmd_present_box_is_new.hpp"
#include "commands/cmd_present_box_delete_new_flag.hpp"
#include "commands/cmd_get_daily_reward.hpp"
#include "commands/cmd_challenge_task_get_list.hpp"
#include "commands/cmd_challenge_task_decide.hpp"
#include "commands/cmd_challenge_task_progress.hpp"
#include "commands/cmd_challenge_task_complete.hpp"
#include "commands/cmd_challenge_task_cancel.hpp"
#include "commands/cmd_wicked_get_text.hpp"
#include "commands/cmd_wicked_report.hpp"
#include "commands/cmd_boost_check_active.hpp"
#include "commands/cmd_event_shop_get_point.hpp"
#include "commands/cmd_event_shop_get_list.hpp"
#include "commands/cmd_event_shop_exchange.hpp"
#include "commands/cmd_event_get_info.hpp"
#include "commands/cmd_event_get_ranking.hpp"
#include "commands/cmd_event_get_friend_ranking.hpp"
#include "commands/cmd_event_get_around_ranking.hpp"
#include "commands/cmd_event_catalog_reward_purchase.hpp"
#include "commands/cmd_event_reward_receive.hpp"
#include "commands/cmd_send_statistics_data.hpp"
#include "commands/cmd_statistics_matching_time.hpp"
#include "commands/cmd_defense_mission_start.hpp"
#include "commands/cmd_defense_mission_cancel.hpp"
#include "commands/cmd_defense_mission_get_history_list.hpp"
#include "commands/cmd_defense_mission_get_history_detail.hpp"
#include "commands/cmd_coop_lobby_end.hpp"
#include "commands/cmd_user_flag_set.hpp"
#include "commands/cmd_get_friend_headers.hpp"
#include "commands/cmd_craft_bp_lottery.hpp"
#include "commands/cmd_craft_deliver.hpp"
#include "commands/cmd_survive_supply_lottery.hpp"
#include "commands/cmd_craft_open_perk.hpp"
#include "commands/cmd_craft_grade_up.hpp"
#include "commands/cmd_craft_reforge.hpp"
#include "commands/cmd_craft_obtain_option.hpp"
#include "commands/cmd_craft_check_crafted.hpp"
#include "commands/cmd_craft_check_crafted_all.hpp"
#include "commands/cmd_craft_invest_to_player.hpp"
#include "commands/cmd_inventory_save.hpp"
#include "commands/cmd_bgm_my_list_save.hpp"
#include "commands/cmd_get_friend_loadout.hpp"

namespace emulator::ssd
{
	main_handler::main_handler()
	{
		this->set_platform("ssdstm/main");
		this->register_handler<cmd_auth_steamticket>("CMD_AUTH_STEAMTICKET");
		this->register_handler<cmd_reqauth_https>("CMD_REQAUTH_HTTPS");
		this->register_handler<cmd_send_ipandport>("CMD_SEND_IPANDPORT");
		this->register_handler<cmd_get_playerlist>("CMD_GET_PLAYERLIST");
		this->register_handler<cmd_set_currentplayer>("CMD_SET_CURRENTPLAYER");
		this->register_handler<cmd_create_player>("CMD_CREATE_PLAYER");
		this->register_handler<cmd_gdpr_check>("CMD_GDPR_CHECK");
		this->register_handler<cmd_reqauth_sessionsvr>("CMD_REQAUTH_SESSIONSVR");
		this->register_handler<cmd_send_heartbeat>("CMD_SEND_HEARTBEAT");
		this->register_handler<cmd_error_info_send>("CMD_ERROR_INFO_SEND");
		this->register_handler<cmd_error_lang_id_send>("CMD_ERROR_LANG_ID_SEND");
		this->register_handler<cmd_get_informationlist>("CMD_GET_INFORMATIONLIST");
		this->register_handler<cmd_update_session>("CMD_UPDATE_SESSION");
		this->register_handler<cmd_svcoin_get_balance>("CMD_SVCOIN_GET_BALANCE");
		this->register_handler<cmd_check_consume_transaction>("CMD_CHECK_CONSUME_TRANSACTION");
		this->register_handler<cmd_start_consume_transaction>("CMD_START_CONSUME_TRANSACTION");
		this->register_handler<cmd_commit_consume_transaction>("CMD_COMMIT_CONSUME_TRANSACTION");
		this->register_handler<cmd_steam_shop_open>("CMD_STEAM_SHOP_OPEN");
		this->register_handler<cmd_steam_shop_approve>("CMD_STEAM_SHOP_APPROVE");
		this->register_handler<cmd_steam_shop_get_item_list>("CMD_STEAM_SHOP_GET_ITEM_LIST");
		this->register_handler<cmd_purchase_player_slot>("CMD_PURCHASE_PLAYER_SLOT");
		this->register_handler<cmd_purchase_deploy_team>("CMD_PURCHASE_DEPLOY_TEAM");
		this->register_handler<cmd_purchase_gesture>("CMD_PURCHASE_GESTURE");
		this->register_handler<cmd_purchase_marker>("CMD_PURCHASE_MARKER");
		this->register_handler<cmd_purchase_radio>("CMD_PURCHASE_RADIO");
		this->register_handler<cmd_purchase_load_out>("CMD_PURCHASE_LOAD_OUT");
		this->register_handler<cmd_purchase_calc_deploy_reduction_price>("CMD_PURCHASE_CALC_DEPLOY_REDUCTION_PRICE");
		this->register_handler<cmd_purchase_deploy_reduction>("CMD_PURCHASE_DEPLOY_REDUCTION");
		this->register_handler<cmd_purchase_additional_storage>("CMD_PURCHASE_ADDITIONAL_STORAGE");
		this->register_handler<cmd_purchase_calc_defense_mission_reduction_price>("CMD_PURCHASE_CALC_DEFENSE_MISSION_REDUCTION_PRICE");
		this->register_handler<cmd_purchase_defense_mission_reduction>("CMD_PURCHASE_DEFENSE_MISSION_REDUCTION");
		this->register_handler<cmd_purchase_get_product_list>("CMD_PURCHASE_GET_PRODUCT_LIST");
		this->register_handler<cmd_purchase_shop_get_item_list>("CMD_PURCHASE_SHOP_GET_ITEM_LIST");
		this->register_handler<cmd_purchase_shop_buy_item>("CMD_PURCHASE_SHOP_BUY_ITEM");
		this->register_handler<cmd_purchase_get_history>("CMD_PURCHASE_GET_HISTORY");
		this->register_handler<cmd_boost_add_confirm>("CMD_BOOST_ADD_CONFIRM");
		this->register_handler<cmd_coop_mission_result>("CMD_COOP_MISSION_RESULT");
		this->register_handler<cmd_matching_createjoin_room>("CMD_MATCHING_CREATEJOIN_ROOM");
		this->register_handler<cmd_matching_leave_room>("CMD_MATCHING_LEAVE_ROOM");
		this->register_handler<cmd_matching_search_room>("CMD_MATCHING_SEARCH_ROOM");
		this->register_handler<cmd_matching_join_room>("CMD_MATCHING_JOIN_ROOM");
		this->register_handler<cmd_matching_get_room_data>("CMD_MATCHING_GET_ROOM_DATA");
		this->register_handler<cmd_matching_get_roommember_data>("CMD_MATCHING_GET_ROOMMEMBER_DATA");
		this->register_handler<cmd_matching_set_room_data_external>("CMD_MATCHING_SET_ROOM_DATA_EXTERNAL");
		this->register_handler<cmd_matching_set_room_data_internal>("CMD_MATCHING_SET_ROOM_DATA_INTERNAL");
		this->register_handler<cmd_matching_set_roommember_data>("CMD_MATCHING_SET_ROOMMEMBER_DATA");
		this->register_handler<cmd_matching_set_room_owner>("CMD_MATCHING_SET_ROOM_OWNER");
		this->register_handler<cmd_matching_get_matching_player_num>("CMD_MATCHING_GET_MATCHING_PLAYER_NUM");
		this->register_handler<cmd_matching_kick_member>("CMD_MATCHING_KICK_MEMBER");
		this->register_handler<cmd_matching_reserve_slot>("CMD_MATCHING_RESERVE_SLOT");
		this->register_handler<cmd_matching_migration_heartbeat>("CMD_MATCHING_MIGRATION_HEARTBEAT");
		this->register_handler<cmd_matching_migration_get_roommember_data>("CMD_MATCHING_MIGRATION_GET_ROOMMEMBER_DATA");
		this->register_handler<cmd_matching_alive_notice>("CMD_MATCHING_ALIVE_NOTICE");
		this->register_handler<cmd_matching_alive_fix>("CMD_MATCHING_ALIVE_FIX");
		this->register_handler<cmd_matching_get_lock_set_room_owner>("CMD_MATCHING_GET_LOCK_SET_ROOM_OWNER");
		this->register_handler<cmd_coop_item_use>("CMD_COOP_ITEM_USE");
		this->register_handler<cmd_coop_mission_end_confirm>("CMD_COOP_MISSION_END_CONFIRM");
		this->register_handler<cmd_coop_mission_start>("CMD_COOP_MISSION_START");
		this->register_handler<cmd_coop_mission_end>("CMD_COOP_MISSION_END");
		this->register_handler<cmd_coop_flag_set>("CMD_COOP_FLAG_SET");
		this->register_handler<cmd_coop_flag_reset>("CMD_COOP_FLAG_RESET");
		this->register_handler<cmd_craft_item>("CMD_CRAFT_ITEM");
		this->register_handler<cmd_get_craft_param>("CMD_GET_CRAFT_PARAM");
		this->register_handler<cmd_checkpoint_save>("CMD_CHECKPOINT_SAVE");
		this->register_handler<cmd_checkpoint_load>("CMD_CHECKPOINT_LOAD");
		this->register_handler<cmd_avatar_save>("CMD_AVATAR_SAVE");
		this->register_handler<cmd_avatar_load>("CMD_AVATAR_LOAD");
		this->register_handler<cmd_mission_start>("CMD_MISSION_START");
		this->register_handler<cmd_mission_end>("CMD_MISSION_END");
		this->register_handler<cmd_mission_replay_end>("CMD_MISSION_REPLAY_END");
		this->register_handler<cmd_flag_mission_start>("CMD_FLAG_MISSION_START");
		this->register_handler<cmd_flag_mission_end>("CMD_FLAG_MISSION_END");
		this->register_handler<cmd_mission_abort>("CMD_MISSION_ABORT");
		this->register_handler<cmd_quest_end>("CMD_QUEST_END");
		this->register_handler<cmd_mission_open>("CMD_MISSION_OPEN");
		this->register_handler<cmd_mission_progress_load>("CMD_MISSION_PROGRESS_LOAD");
		this->register_handler<cmd_event_log_send>("CMD_EVENT_LOG_SEND");
		this->register_handler<cmd_character_set_name>("CMD_CHARACTER_SET_NAME");
		this->register_handler<cmd_character_set_class>("CMD_CHARACTER_SET_CLASS");
		this->register_handler<cmd_server_data_initialize>("CMD_SERVER_DATA_INITIALIZE");
		this->register_handler<cmd_survivor_initialize>("CMD_SURVIVOR_INITIALIZE");
		this->register_handler<cmd_mission_defense_mission_start>("CMD_MISSION_DEFENSE_MISSION_START");
		this->register_handler<cmd_dlc_set_flag>("CMD_DLC_SET_FLAG");
		this->register_handler<cmd_dlc_get_flag>("CMD_DLC_GET_FLAG");
		this->register_handler<cmd_communication_gesture_slot_save>("CMD_COMMUNICATION_GESTURE_SLOT_SAVE");
		this->register_handler<cmd_cheat_send>("CMD_CHEAT_SEND");
		this->register_handler<cmd_base_resource_provide>("CMD_BASE_RESOURCE_PROVIDE");
		this->register_handler<cmd_base_resource_load>("CMD_BASE_RESOURCE_LOAD");
		this->register_handler<cmd_base_resource_consume>("CMD_BASE_RESOURCE_CONSUME");
		this->register_handler<cmd_base_resource_income>("CMD_BASE_RESOURCE_INCOME");
		this->register_handler<cmd_base_resource_draw_water>("CMD_BASE_RESOURCE_DRAW_WATER");
		this->register_handler<cmd_base_resource_update>("CMD_BASE_RESOURCE_UPDATE");
		this->register_handler<cmd_building_create>("CMD_BUILDING_CREATE");
		this->register_handler<cmd_building_load>("CMD_BUILDING_LOAD");
		this->register_handler<cmd_building_relocate>("CMD_BUILDING_RELOCATE");
		this->register_handler<cmd_building_remove>("CMD_BUILDING_REMOVE");
		this->register_handler<cmd_building_damage>("CMD_BUILDING_DAMAGE");
		this->register_handler<cmd_building_repair>("CMD_BUILDING_REPAIR");
		this->register_handler<cmd_building_repair_list>("CMD_BUILDING_REPAIR_LIST");
		this->register_handler<cmd_building_mining_machine_repair>("CMD_BUILDING_MINING_MACHINE_REPAIR");
		this->register_handler<cmd_deploy_get_list>("CMD_DEPLOY_GET_LIST");
		this->register_handler<cmd_crew_load>("CMD_CREW_LOAD");
		this->register_handler<cmd_crew_group_transfer>("CMD_CREW_GROUP_TRANSFER");
		this->register_handler<cmd_crew_banish>("CMD_CREW_BANISH");
		this->register_handler<cmd_crew_add>("CMD_CREW_ADD");
		this->register_handler<cmd_crew_update>("CMD_CREW_UPDATE");
		this->register_handler<cmd_crew_treat>("CMD_CREW_TREAT");
		this->register_handler<cmd_crew_use_item>("CMD_CREW_USE_ITEM");
		this->register_handler<cmd_crew_use_party_item>("CMD_CREW_USE_PARTY_ITEM");
		this->register_handler<cmd_deploy_load_team>("CMD_DEPLOY_LOAD_TEAM");
		this->register_handler<cmd_deploy_save_team>("CMD_DEPLOY_SAVE_TEAM");
		this->register_handler<cmd_deploy_deploy>("CMD_DEPLOY_DEPLOY");
		this->register_handler<cmd_deploy_complete>("CMD_DEPLOY_COMPLETE");
		this->register_handler<cmd_deploy_supply_item>("CMD_DEPLOY_SUPPLY_ITEM");
		this->register_handler<cmd_server_parameter_load>("CMD_SERVER_PARAMETER_LOAD");
		this->register_handler<cmd_server_parameter_get_url_list>("CMD_SERVER_PARAMETER_GET_URL_LIST");
		this->register_handler<cmd_information_get_title>("CMD_INFORMATION_GET_TITLE");
		this->register_handler<cmd_information_get_text>("CMD_INFORMATION_GET_TEXT");
		this->register_handler<cmd_information_get_detail>("CMD_INFORMATION_GET_DETAIL");
		this->register_handler<cmd_present_box_get_list>("CMD_PRESENT_BOX_GET_LIST");
		this->register_handler<cmd_present_box_get_item>("CMD_PRESENT_BOX_GET_ITEM");
		this->register_handler<cmd_present_box_get_all>("CMD_PRESENT_BOX_GET_ALL");
		this->register_handler<cmd_present_box_is_new>("CMD_PRESENT_BOX_IS_NEW");
		this->register_handler<cmd_present_box_delete_new_flag>("CMD_PRESENT_BOX_DELETE_NEW_FLAG");
		this->register_handler<cmd_get_daily_reward>("CMD_GET_DAILY_REWARD");
		this->register_handler<cmd_challenge_task_get_list>("CMD_CHALLENGE_TASK_GET_LIST");
		this->register_handler<cmd_challenge_task_decide>("CMD_CHALLENGE_TASK_DECIDE");
		this->register_handler<cmd_challenge_task_progress>("CMD_CHALLENGE_TASK_PROGRESS");
		this->register_handler<cmd_challenge_task_complete>("CMD_CHALLENGE_TASK_COMPLETE");
		this->register_handler<cmd_challenge_task_cancel>("CMD_CHALLENGE_TASK_CANCEL");
		this->register_handler<cmd_wicked_get_text>("CMD_WICKED_GET_TEXT");
		this->register_handler<cmd_wicked_report>("CMD_WICKED_REPORT");
		this->register_handler<cmd_boost_check_active>("CMD_BOOST_CHECK_ACTIVE");
		this->register_handler<cmd_event_shop_get_point>("CMD_EVENT_SHOP_GET_POINT");
		this->register_handler<cmd_event_shop_get_list>("CMD_EVENT_SHOP_GET_LIST");
		this->register_handler<cmd_event_shop_exchange>("CMD_EVENT_SHOP_EXCHANGE");
		this->register_handler<cmd_event_get_info>("CMD_EVENT_GET_INFO");
		this->register_handler<cmd_event_get_ranking>("CMD_EVENT_GET_RANKING");
		this->register_handler<cmd_event_get_friend_ranking>("CMD_EVENT_GET_FRIEND_RANKING");
		this->register_handler<cmd_event_get_around_ranking>("CMD_EVENT_GET_AROUND_RANKING");
		this->register_handler<cmd_event_catalog_reward_purchase>("CMD_EVENT_CATALOG_REWARD_PURCHASE");
		this->register_handler<cmd_event_reward_receive>("CMD_EVENT_REWARD_RECEIVE");
		this->register_handler<cmd_send_statistics_data>("CMD_SEND_STATISTICS_DATA");
		this->register_handler<cmd_statistics_matching_time>("CMD_STATISTICS_MATCHING_TIME");
		this->register_handler<cmd_defense_mission_start>("CMD_DEFENSE_MISSION_START");
		this->register_handler<cmd_defense_mission_cancel>("CMD_DEFENSE_MISSION_CANCEL");
		this->register_handler<cmd_defense_mission_get_history_list>("CMD_DEFENSE_MISSION_GET_HISTORY_LIST");
		this->register_handler<cmd_defense_mission_get_history_detail>("CMD_DEFENSE_MISSION_GET_HISTORY_DETAIL");
		this->register_handler<cmd_coop_lobby_end>("CMD_COOP_LOBBY_END");
		this->register_handler<cmd_user_flag_set>("CMD_USER_FLAG_SET");
		this->register_handler<cmd_get_friend_headers>("CMD_GET_FRIEND_HEADERS");
		this->register_handler<cmd_craft_bp_lottery>("CMD_CRAFT_BP_LOTTERY");
		this->register_handler<cmd_craft_deliver>("CMD_CRAFT_DELIVER");
		this->register_handler<cmd_survive_supply_lottery>("CMD_SURVIVE_SUPPLY_LOTTERY");
		this->register_handler<cmd_craft_open_perk>("CMD_CRAFT_OPEN_PERK");
		this->register_handler<cmd_craft_grade_up>("CMD_CRAFT_GRADE_UP");
		this->register_handler<cmd_craft_reforge>("CMD_CRAFT_REFORGE");
		this->register_handler<cmd_craft_obtain_option>("CMD_CRAFT_OBTAIN_OPTION");
		this->register_handler<cmd_craft_check_crafted>("CMD_CRAFT_CHECK_CRAFTED");
		this->register_handler<cmd_craft_check_crafted_all>("CMD_CRAFT_CHECK_CRAFTED_ALL");
		this->register_handler<cmd_craft_invest_to_player>("CMD_CRAFT_INVEST_TO_PLAYER");
		this->register_handler<cmd_inventory_save>("CMD_INVENTORY_SAVE");
		this->register_handler<cmd_bgm_my_list_save>("CMD_BGM_MY_LIST_SAVE");
		this->register_handler<cmd_get_friend_loadout>("CMD_GET_FRIEND_LOADOUT");
	}
}
