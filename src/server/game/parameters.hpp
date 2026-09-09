#pragma once

#include "parameter/base_parameter.hpp"
#include "parameter/defense_game_base_digging.hpp"
#include "parameter/defense_game_embedded_mission.hpp"
#include "parameter/defense_game_event_mission.hpp"
#include "parameter/defense_game_quest.hpp"
#include "parameter/ssd_base_defense_settings.hpp"
#include "parameter/ssd_buff_parameter.hpp"
#include "parameter/ssd_buff_parameter_debug.hpp"
#include "parameter/ssd_buff_parameter_opt.hpp"
#include "parameter/ssd_building_parameter_table.hpp"
#include "parameter/ssd_collection_items.hpp"
#include "parameter/ssd_combat_deploy_parameter_table.hpp"
#include "parameter/ssd_crew_generator_table.hpp"
#include "parameter/ssd_damage_parameter.hpp"
#include "parameter/ssd_damage_parameter_debug.hpp"
#include "parameter/ssd_damage_parameter_opt.hpp"
#include "parameter/ssd_dlc_mission_settings.hpp"
#include "parameter/ssd_equip_parameters.hpp"
#include "parameter/ssd_equip_parameters_debug.hpp"
#include "parameter/ssd_equip_parameters_opt.hpp"
#include "parameter/ssd_npc_level_parameters.hpp"
#include "parameter/ssd_player_parameters.hpp"
#include "parameter/ssd_replay_mission_info_opt.hpp"
#include "parameter/ssd_sbm_parameters.hpp"
#include "parameter/ssd_sbm_parameters_debug.hpp"
#include "parameter/ssd_sbm_parameters_opt.hpp"
#include "parameter/ssd_ui_coop_mission_info.hpp"
#include "parameter/ssd_walker_gear_parameters.hpp"
#include "parameter/ssd_weapon_parameters.hpp"
#include "parameter/ssd_weapon_parameters_debug.hpp"
#include "parameter/ssd_weapon_parameters_opt.hpp"

namespace game
{
	struct parameters_table_t
	{
		std::shared_ptr<parameters::ssd_building_parameter_table> ssd_building_parameter_table;
		std::shared_ptr<parameters::defense_game_base_digging> defense_game_base_digging;
		std::shared_ptr<parameters::defense_game_quest> defense_game_quest;
		std::shared_ptr<parameters::ssd_base_defense_settings> ssd_base_defense_settings;
		std::shared_ptr<parameters::ssd_buff_parameter> ssd_buff_parameter;
		std::shared_ptr<parameters::ssd_buff_parameter_debug> ssd_buff_parameter_debug;
		std::shared_ptr<parameters::ssd_collection_items> ssd_collection_items;
		std::shared_ptr<parameters::ssd_combat_deploy_parameter_table> ssd_combat_deploy_parameter_table;
		std::shared_ptr<parameters::ssd_equip_parameters> ssd_equip_parameters;
		std::shared_ptr<parameters::ssd_equip_parameters_debug> ssd_equip_parameters_debug;
		std::shared_ptr<parameters::ssd_equip_parameters_opt> ssd_equip_parameters_opt;
		std::shared_ptr<parameters::ssd_player_parameters> ssd_player_parameters;
		std::shared_ptr<parameters::ssd_replay_mission_info_opt> ssd_replay_mission_info_opt;
		std::shared_ptr<parameters::ssd_walker_gear_parameters> ssd_walker_gear_parameters;
		std::shared_ptr<parameters::ssd_weapon_parameters> ssd_weapon_parameters;
		std::shared_ptr<parameters::ssd_weapon_parameters_debug> ssd_weapon_parameters_debug;
		std::shared_ptr<parameters::ssd_weapon_parameters_opt> ssd_weapon_parameters_opt;
		std::shared_ptr<parameters::ssd_crew_generator_table> ssd_crew_generator_table;
		std::shared_ptr<parameters::ssd_npc_level_parameters> ssd_npc_level_parameters;
		std::shared_ptr<parameters::ssd_damage_parameter> ssd_damage_parameter;
		std::shared_ptr<parameters::ssd_damage_parameter_debug> ssd_damage_parameter_debug;
		std::shared_ptr<parameters::ssd_damage_parameter_opt> ssd_damage_parameter_opt;
		std::shared_ptr<parameters::defense_game_embedded_mission> defense_game_embedded_mission;
		std::shared_ptr<parameters::ssd_sbm_parameters> ssd_sbm_parameters;
		std::shared_ptr<parameters::ssd_sbm_parameters_debug> ssd_sbm_parameters_debug;
		std::shared_ptr<parameters::ssd_sbm_parameters_opt> ssd_sbm_parameters_opt;
		std::shared_ptr<parameters::defense_game_event_mission> defense_game_event_mission;
		std::shared_ptr<parameters::ssd_buff_parameter_opt> ssd_buff_parameter_opt;
		std::shared_ptr<parameters::ssd_dlc_mission_settings> ssd_dlc_mission_settings;
		std::shared_ptr<parameters::ssd_ui_coop_mission_info> ssd_ui_coop_mission_info;
	};

	extern parameters_table_t parameters_table;
	extern std::vector<std::shared_ptr<parameters::base_parameter>> parameters_list;
	extern std::unordered_map<std::string, std::shared_ptr<parameters::base_parameter>> parameters_map;

	void initialize_parameters_table();
}
