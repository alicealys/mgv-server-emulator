#pragma once

#include "../database.hpp"
#include "game/game.hpp"
#include "game/parameters.hpp"
#include "utils/encoding.hpp"
#include "../utils.hpp"

namespace database::players
{
	constexpr const auto max_player_count = 4u;
	constexpr const auto max_loadout_count = 9u;
	constexpr const auto initial_loadout_count = 4u;

#pragma pack(push, 1)
	struct avatar_internal_t
	{
		std::uint8_t accessory;
		std::uint8_t beard_length;
		std::uint8_t beard_style;
		std::uint8_t eyebrow_length;
		std::uint8_t eyebrow_style;
		std::uint8_t hair_color;
		std::uint8_t hair_style;
		std::uint8_t left_eye_brightness;
		std::uint8_t left_eye_color;
		std::uint8_t player_parts_type;
		std::uint8_t player_type;
		std::uint8_t race;
		std::uint8_t race_color;
		std::uint8_t race_type;
		std::uint8_t race_variation;
		std::uint8_t right_eye_brightness;
		std::uint8_t right_eye_color;
		std::uint8_t tattoo;
		std::uint8_t tattoo_color;
		std::uint8_t voice;
		std::uint8_t voice_pitch;
		std::uint8_t motion_frame_list[60];
		char name[64];

		bool parse(json::value& data);
		void to_json(json::value& data) const;
	};

	struct nonstackable_item_t
	{
		struct option_t
		{
			std::uint32_t option_id;
			std::uint8_t obtained;
		};

		struct perk_t
		{
			std::uint32_t perk_id;
			std::uint8_t perk_level;
		};

		std::uint8_t color;
		std::uint8_t color2;
		std::uint8_t flag;
		std::uint16_t grade;
		std::uint16_t inventory_index;
		std::uint16_t life;
		std::uint16_t life_max;
		std::uint16_t option_slot;
		std::uint16_t spec;
		std::uint32_t obtain_order;
		std::uint32_t production_id;
		option_t option_list[8];
		perk_t perk_list[5];

		void initialize(const std::shared_ptr<game::production_t>& production, const std::shared_ptr<game::potential_t>& potential);
		bool parse(json::value& data);
		void to_json(json::value& data) const;
	};

	struct stackable_item_t
	{
		struct parseable
		{
			std::uint8_t flag;
			std::uint8_t inventory_type;
			std::uint8_t cbox_index;
			std::uint8_t inventory_index;
			std::uint8_t obtain_order;
			std::uint8_t damaged_in_count;
			std::uint32_t count;
		};

		std::uint32_t flag : 1;
		std::uint32_t inventory_type : 1;
		std::uint32_t cbox_index : 3;
		std::uint32_t inventory_index : 11;
		std::uint32_t obtain_order : 8;
		std::uint32_t damaged_in_count : 8;
		std::uint32_t production_index : 15;
		std::uint32_t count : 17;

		inline std::uint32_t get_production_id() const
		{
			if (this->production_index > game::parameters_table.ssd_sbm_parameters->productions_list.size())
			{
				return 0u;
			}

			const auto& prod = game::parameters_table.ssd_sbm_parameters->productions_list[this->production_index];
			if (prod == nullptr)
			{
				return 0u;
			}

			return prod->id;
		}

		bool parse(json::value& data);
		void to_json(json::value& data) const;
	};

	struct loadout_t
	{
		struct item_t
		{
			std::uint32_t count;
			std::uint32_t idx;
		};

		struct weapon_t
		{
			std::uint16_t ammo_count;
			std::uint16_t ammo_idx;
			std::uint16_t init_ammo_count;
			std::uint16_t init_ammo_idx;
			std::uint16_t inventory_index;
		};

		struct gear_info_t
		{
			std::uint16_t arm_inventory_index;
			std::uint16_t body_inventory_index;
			std::uint16_t ext_head_production_idx;
			std::uint16_t ext_suit_production_idx;
			std::uint16_t head_inventory_index;
			std::uint16_t leg_inventory_index;
		};

		struct skill_t
		{
			std::uint16_t slot[8];
		};

		bool valid;
		std::uint16_t class_info;
		gear_info_t gear_info;
		weapon_t main_weapon_list[3];
		weapon_t sub_weapon_list[3];
		skill_t skill_list[5];
		item_t gadget_list[16];
		std::uint16_t survival_list[8];
		item_t porch_list[20];
		char name[64];

		void initialize(const std::uint32_t index = 0u);
		bool parse(json::value& data, std::uint32_t& index);
		void to_json(json::value& data, const std::uint32_t index = 0u) const;
	};

	struct mission_info_t
	{
		struct status_buffer_t
		{
			std::uint32_t buffer_type;
			std::uint32_t remaining_time;
		};

		struct replay_info_t
		{
			std::uint8_t is_replay_mission;
			std::uint8_t replay_mission_difficalty;
			std::uint32_t replay_mission_id;
			std::uint32_t replay_mission_return_location_code;
			std::uint32_t replay_mission_return_mission_code;

			bool parse(json::value& data);
			void to_json(json::value& data) const;
		};

		std::uint16_t equipment_slot; // 206,
		std::uint32_t flag_mission_code; // 40070,
		std::uint16_t flag_mission_sequence_number; // 1,
		std::uint16_t hunger; // 957,
		std::uint16_t hunger_max_keep_time; // 0,
		std::uint16_t injury_whole; // 0,
		std::uint16_t life; // 1358,
		std::uint16_t location_code; // 15,
		std::uint16_t mission_code; // 30010,
		std::uint16_t oxygen; // 1000,
		std::uint16_t sequence_number; // 9,
		std::uint16_t story_sequence_number; // 9,
		std::uint16_t stamina; // 2148,
		std::uint16_t temp_body_id; // 0,
		std::uint16_t temp_crew_type_code; // 0,
		std::uint16_t temp_face_id; // 0,
		std::uint16_t temp_first_name_id; // 0,
		std::uint16_t temp_last_name_id; // 0,
		std::uint16_t temp_race_id; // 0,
		std::uint16_t temp_sex_id; // 0,
		std::uint16_t temp_unique_type_code; // 0,
		std::uint16_t temp_voice_type; // 0,
		std::uint16_t thirst; // 1808,
		std::uint16_t thirst_max_keep_time; // 0,
		std::uint16_t tiredness; // 547,
		std::uint16_t weather; // 0
		std::int32_t pos_x; // -4508,
		std::int32_t pos_y; // 2880,
		std::int32_t pos_z; // 22444,
		std::int32_t rot_y; // -89,
		std::uint64_t clock; // 60902697500,
		std::uint64_t survival_sec; // 33595,
		std::uint16_t injury_part[8]; // 0
		std::uint16_t injury_recovery_time[32]; // 0
		status_buffer_t status_buffer[32];
		std::uint8_t vars[640]; // 
		replay_info_t replay_mission_info;

		void initialize();
		bool parse(json::value& data);
		void to_json(json::value& data) const;
	};

	struct player_inventory_t
	{
		std::uint32_t energy;
		std::uint32_t class_opened;
		std::uint32_t oxygen_convert_count;
		std::uint32_t energy_invested[5];
		std::uint16_t cbox_history[3];
		std::uint16_t cbox_location[3];
		std::uint16_t cbox_updated[3];
		std::int32_t cbox_pos[12];
		std::uint8_t event_obtained[2];
		std::uint8_t skill_status[255];
		std::uint8_t survival_new[32];
		std::uint8_t survival_obtained[32];
		std::uint8_t survival_slot_new[2];

		void initialize();
		bool parse(json::value& data, std::uint16_t& nameplate);
		bool parse_save(json::value& data, std::uint16_t& nameplate);
		void to_json(json::value& data, const std::uint16_t nameplate = 0u) const;
		void set_survival_obtained(const std::uint32_t index, bool obtained);

	};

	struct gimmick_save_data_t
	{
		struct instant_t
		{
			std::uint8_t data[2048];
		};

		struct permanent_t
		{
			std::uint8_t data[512];
		};

		struct resource_event_t
		{
			std::uint8_t data[896];
		};

		struct resource_normal_t
		{
			std::uint8_t data[1344];
		};

		struct resource_rare_t
		{
			std::uint8_t data[224];
		};

		struct resource_shared_t
		{
			std::uint8_t data[224];
		};

		instant_t instant[4];
		permanent_t permanent[4];
		resource_event_t resource_event[4];
		resource_normal_t resource_normal[4];
		resource_rare_t resource_rare[4];
		resource_shared_t resource_shared[4];

		bool parse(json::value& parse);
		void to_json(json::value& data, const std::uint32_t map_location) const;
	};

	struct loadout_list_t
	{
		loadout_t list[max_loadout_count];
	};

	struct gimmick_resource_info_t
	{
		std::uint32_t resource_event_tail;
		std::uint32_t resource_normal_tail;
		std::uint32_t resource_rare_tail;
		std::uint32_t resource_shared_tail;

		bool parse(json::value& data);
		void to_json(json::value& data) const;
	};

	struct gimmick_timer_info_t
	{
		std::uint16_t resource_timer_global_afghan;
		std::uint16_t resource_timer_global_africa;
		std::uint16_t resource_timer_stock_afghan;
		std::uint16_t resource_timer_stock_africa;

		bool parse(json::value& data);
		void to_json(json::value& data) const;
	};

	struct gimmick_info_internal_t
	{
		gimmick_resource_info_t resource_afghan;
		gimmick_resource_info_t resource_africa;
		gimmick_timer_info_t timer;
	};

	struct base_resources_t
	{
		struct base_resource_params_t
		{
			std::uint32_t bad_status_1_risk;
			std::uint32_t bad_status_2_risk;
			std::uint32_t bad_status_3_risk;
			std::uint32_t bad_status_4_risk;
			std::uint32_t clean_water;
			std::uint32_t dirty_water;
			std::uint32_t food;
			std::uint32_t medical_supplies;
			std::uint32_t party_item_id;
			std::uint32_t party_item_updates;
			std::uint32_t total_number_of_updates;
		};

		std::uint32_t animals[57];
		std::uint32_t resource_counts[32];
		std::uint64_t next_update_time;
		std::uint64_t update_remaining_time;
		base_resource_params_t params;

		bool parse_base(json::value& base);
		bool parse_counts(json::value& count);
		bool parse_animals(json::value& animals);
		void to_json(json::value& data) const;
	};

	struct inventory_resource_t
	{
		struct parseable
		{
			std::uint8_t flag;
			std::uint8_t inventory_type;
			std::uint8_t cbox_index;
			std::uint8_t inventory_index;
			std::uint8_t obtain_order;
			std::uint8_t damaged_in_count;
			std::uint32_t count;
		};

		std::uint32_t flag : 1;
		std::uint32_t inventory_type : 1;
		std::uint32_t cbox_index : 3;
		std::uint32_t inventory_index : 11;
		std::uint32_t obtain_order : 8;
		std::uint32_t damaged_in_count : 8;
		std::uint32_t resource_index : 15;
		std::uint32_t count : 17;
		
		inline std::uint32_t get_resource_id() const
		{
			if (this->resource_index > game::parameters_table.ssd_sbm_parameters->resources_list.size())
			{
				return 0u;
			}

			const auto& res = game::parameters_table.ssd_sbm_parameters->resources_list[this->resource_index];
			if (res == nullptr)
			{
				return 0u;
			}

			return res->id;
		}

		bool parse(json::value& data);
		void to_json(json::value& data) const;
	};

	struct battle_pack_t
	{
		struct parseable
		{
			std::uint8_t flag;
			std::uint8_t inventory_type;
			std::uint8_t cbox_index;
			std::uint8_t inventory_index;
			std::uint8_t obtain_order;
			std::uint32_t count;
		};

		std::uint32_t flag : 1;
		std::uint32_t inventory_type : 1;
		std::uint32_t cbox_index : 3;
		std::uint32_t inventory_index : 7;
		std::uint32_t obtain_order : 7;
		std::uint32_t bp_index : 7;
		std::uint32_t count : 17;
		std::uint32_t unused : 15;

		inline std::uint32_t get_battle_pack_id() const
		{
			if (this->bp_index > game::parameters_table.ssd_sbm_parameters->battle_pack_list.size())
			{
				return 0u;
			}

			const auto& res = game::parameters_table.ssd_sbm_parameters->battle_pack_list[this->bp_index];
			if (res == nullptr)
			{
				return 0u;
			}

			return res->id;
		}

		bool parse(json::value& data);
		void to_json(json::value& data) const;
	};

	enum play_record_type_t
	{
		play_record_index_0 = 0,
		play_record_index_1 = 1,
		play_record_index_2 = 2,
		play_record_index_3 = 3,
		play_record_index_4 = 4,
		play_record_index_5 = 5,
		play_record_index_6 = 6,
		play_record_index_7 = 7,
		play_record_index_8 = 8,
		play_record_index_9 = 9,
		play_record_index_10 = 10,
		play_record_index_11 = 11,
		play_record_index_12 = 12,
		play_record_index_13 = 13,
		play_record_index_14 = 14,
		play_record_index_15 = 15,
		play_record_index_16 = 16,
		play_record_index_17 = 17,
		play_record_index_18 = 18,
		play_record_index_19 = 19,
		play_record_index_20 = 20,
		play_record_index_21 = 21,
		play_record_index_22 = 22,
		play_record_index_23 = 23,
		play_record_index_24 = 24,
		play_record_index_25 = 25,
		play_record_index_26 = 26,
		play_record_index_27 = 27,
		play_record_index_28 = 28,
		play_record_index_29 = 29,
		play_record_index_30 = 30,
		play_record_index_31 = 31,
		play_record_index_32 = 32,
		play_record_index_33 = 33,
		play_record_index_34 = 34,
		play_record_index_35 = 35,
		play_record_index_36 = 36,
		play_record_index_37 = 37,
		play_record_index_38 = 38,
		play_record_index_39 = 39,
		play_record_index_40 = 40,
		play_record_index_41 = 41,
		play_record_index_42 = 42,
		play_record_index_43 = 43,
		play_record_index_44 = 44,
		play_record_index_45 = 45,
		play_record_index_46 = 46,
		play_record_index_47 = 47,
		play_record_index_48 = 48,
		play_record_index_49 = 49,
		play_record_index_50 = 50,
		play_record_index_51 = 51,
		play_record_index_52 = 52,
		play_record_index_53 = 53,
		play_record_index_54 = 54,
		play_record_index_55 = 55,
		play_record_index_56 = 56,
		play_record_index_57 = 57,
		play_record_index_58 = 58,
		play_record_index_59 = 59,
		play_record_index_60 = 60,
		play_record_index_61 = 61,
		play_record_index_62 = 62,
		play_record_index_63 = 63,
		play_record_index_64 = 64,
		play_record_index_65 = 65,
		play_record_index_66 = 66,
		play_record_index_67 = 67,
		play_record_index_68 = 68,
		play_record_index_69 = 69,
		play_record_index_70 = 70,
		play_record_index_71 = 71,
		play_record_index_72 = 72,
		play_record_index_73 = 73,
		play_record_index_74 = 74,
		play_record_index_75 = 75,
		play_record_index_76 = 76,
		play_record_index_77 = 77,
		play_record_index_78 = 78,
		play_record_index_79 = 79,
		play_record_index_80 = 80,
		play_record_index_81 = 81,
		play_record_index_82 = 82,
		play_record_index_83 = 83,
		play_record_index_84 = 84,
		play_record_index_85 = 85,
		play_record_index_86 = 86,
		play_record_index_87 = 87,
		play_record_index_88 = 88,
		play_record_index_89 = 89,
		play_record_index_90 = 90,
		play_record_index_91 = 91,
		play_record_index_92 = 92,
		play_record_index_93 = 93,
		play_record_index_94 = 94,
		play_record_index_95 = 95,
		play_record_index_96 = 96,
		play_record_index_97 = 97,
		play_record_index_98 = 98,
		play_record_index_99 = 99,
		play_record_index_100 = 100,
		play_record_index_101 = 101,
		play_record_index_102 = 102,
		play_record_index_103 = 103,
		play_record_index_104 = 104,
		play_record_index_105 = 105,
		play_record_index_106 = 106,
		play_record_index_107 = 107,
		play_record_index_108 = 108,
		play_record_index_109 = 109,
		play_record_index_110 = 110,
		play_record_index_111 = 111,
		play_record_index_112 = 112,
		play_record_index_113 = 113,
		play_record_index_114 = 114,
		play_record_index_115 = 115,
		play_record_index_116 = 116,
		play_record_index_117 = 117,
		play_record_index_118 = 118,
		play_record_index_119 = 119,
		play_record_index_120 = 120,
		play_record_index_121 = 121,
		play_record_index_122 = 122,
		play_record_index_123 = 123,
		play_record_index_124 = 124,
		play_record_index_125 = 125,
		play_record_index_126 = 126,
		play_record_index_127 = 127,
		play_record_index_128 = 128,
		play_record_index_129 = 129,
		play_record_index_130 = 130,
		play_record_index_131 = 131,
		play_record_index_132 = 132,
		play_record_index_133 = 133,
		play_record_index_134 = 134,
		play_record_index_135 = 135,
		play_record_index_136 = 136,
		play_record_index_137 = 137,
		play_record_index_138 = 138,
		play_record_index_139 = 139,
		play_record_index_140 = 140,
		play_record_index_141 = 141,
		play_record_index_142 = 142,
		play_record_index_143 = 143,
		play_record_index_144 = 144,
		play_record_index_145 = 145,
		play_record_index_146 = 146,
		play_record_index_147 = 147,
		play_record_index_148 = 148,
		play_record_index_149 = 149,
		play_record_index_150 = 150,
		play_record_index_151 = 151,
		play_record_index_152 = 152,
		play_record_index_153 = 153,
		play_record_index_154 = 154,
		play_record_index_155 = 155,
		play_record_index_156 = 156,
		play_record_index_157 = 157,
		play_record_index_158 = 158,
		play_record_index_159 = 159,
		play_record_index_160 = 160,
		play_record_index_161 = 161,
		play_record_index_162 = 162,
		play_record_index_163 = 163,
		play_record_index_164 = 164,
		play_record_index_165 = 165,
		play_record_index_166 = 166,
		play_record_index_167 = 167,
		play_record_index_168 = 168,
		play_record_index_169 = 169,
		play_record_index_170 = 170,
		play_record_index_171 = 171,
		play_record_index_172 = 172,
		play_record_index_173 = 173,
		play_record_index_174 = 174,
		play_record_index_175 = 175,
		play_record_index_176 = 176,
		play_record_index_177 = 177,
		play_record_index_178 = 178,
		play_record_index_179 = 179,
		play_record_index_180 = 180,
		play_record_index_181 = 181,
		play_record_index_182 = 182,
		play_record_index_183 = 183,
		play_record_index_184 = 184,
		play_record_index_185 = 185,
		play_record_index_186 = 186,
		play_record_index_187 = 187,
		play_record_index_188 = 188,
		play_record_index_189 = 189,
		play_record_index_190 = 190,
		play_record_additional_index_0 = 0,
		play_record_additional_index_1 = 1,
		play_record_additional_index_2 = 2,
		play_record_additional_index_3 = 3,
		play_record_additional_index_4 = 4,
		play_record_additional_index_5 = 5,
		play_record_additional_index_6 = 6,
		play_record_additional_index_7 = 7,
		play_record_additional_index_8 = 8,
		play_record_additional_index_9 = 9,
		play_record_additional_index_10 = 10,
		play_record_additional_index_11 = 11,
		play_record_additional_index_12 = 12,
		play_record_additional_index_13 = 13,
		play_record_additional_index_14 = 14,
		play_record_additional_index_15 = 15,
		play_record_additional_index_16 = 16,
		play_record_additional_index_17 = 17,
		play_record_additional_index_18 = 18,
		play_record_additional_index_19 = 19,
		play_record_additional_index_20 = 20,
		play_record_additional_index_21 = 21,
		play_record_additional_index_22 = 22,
		play_record_additional_index_23 = 23,
		play_record_additional_index_24 = 24,
		play_record_additional_index_25 = 25,
		play_record_additional_index_26 = 26,
		play_record_additional_index_27 = 27,
		play_record_additional_index_28 = 28,
		play_record_additional_index_29 = 29,
		play_record_additional_index_30 = 30,
		play_record_additional_index_31 = 31,
		play_record_additional_index_32 = 32,
		play_record_additional_index_33 = 33,
		play_record_additional_index_34 = 34,
		play_record_additional_index_35 = 35,
		play_record_additional_index_36 = 36,
		play_record_additional_index_37 = 37,
		play_record_additional_index_38 = 38,
		play_record_additional_index_39 = 39,
		play_record_additional_index_40 = 40,
		play_record_additional_index_41 = 41,
		play_record_additional_index_42 = 42,
		play_record_additional_index_43 = 43,
		play_record_additional_index_44 = 44,
		play_record_additional_index_45 = 45,
	};

	struct player_play_record_t
	{
		std::uint32_t first[191];
		std::uint32_t additional[46];

		bool parse_save(json::value& first_save, json::value& additional_save);
	};

	struct story_unlock_info_t
	{
		struct tips_open_info_t
		{
			std::uint8_t value[128];
			bool parse(json::value& data);
			void to_json(json::value& data) const;
		};

		std::uint32_t demo_open_flag;
		std::uint32_t facility_new_flag;
		std::uint32_t marker_map_location;
		std::uint32_t oxygen_supply_unlock;
		std::uint32_t story_sequence_number;
		std::uint8_t fast_travel_unlock[8];
		std::uint8_t marker_afghan[522];
		std::uint8_t marker_africa[522];
		tips_open_info_t tips_open_info;

		void set_story_sequence_number(const std::uint32_t story_sequence_number);

		bool parse(json::value& data);
		void to_json(json::value& data) const;
	};

	constexpr const auto building_grid_size = 32u;
	
	enum edge_type_t
	{
		edge_type_center = 0,
		edge_type_upper = 1,
		edge_type_left = 2,
		edge_type_count = 3,
	};

	struct building_info_t
	{
		struct cell_edge_t
		{
			std::uint8_t rotation;
			std::uint16_t extra_data;
			std::uint16_t life;
			std::uint16_t max_life;
			std::uint16_t production_index;
			std::uint32_t completion_remaining_time;
			std::uint32_t recovery_time;

			bool parse(json::value& data, std::uint32_t& row, std::uint32_t& column, const std::uint32_t type);
			void to_json(json::value& data, const std::uint32_t row, const std::uint32_t column, const std::uint32_t type) const;
		};

		struct farming_info_t
		{
			std::uint8_t animal[8];
			std::uint8_t item[10];

			bool parse(json::value& data, std::uint32_t& row, std::uint32_t& column);
			void to_json(json::value& data, const std::uint32_t row, const std::uint32_t column) const;
		};

		struct cell_t
		{
			farming_info_t farming_info;
			cell_edge_t edges[edge_type_count];
		};

		cell_t cells[building_grid_size][building_grid_size];

		bool parse_type(json::value& data, const std::uint32_t type);
		bool parse_farming(json::value& data);
		bool parse(json::value& data, const bool is_diff);

		void to_json(json::value& data) const;
		void to_json_farming(json::value& data) const;

		void load_default(const std::uint32_t map_location);
	};

	struct crew_levels_internal_t
	{
		std::uint8_t base_defense;
		std::uint8_t combat_deploy;
		std::uint8_t develop;
		std::uint8_t food;
		std::uint8_t medic;
		std::uint8_t plant;

		bool parse(json::value& data);
		void to_json(json::value& data) const;
	};

	struct defense_mission_info_internal_t
	{
		struct status_t
		{
			std::uint32_t mining_machine_life;

			bool parse(json::value& data);
			void to_json(json::value& data) const;
		};

		struct parameter_t
		{
			std::uint32_t flag;
			std::uint32_t threat;
			std::uint32_t threat_threshold;
			std::uint32_t attack_time;

			bool parse(json::value& data);
			void to_json(json::value& data) const;
		};

		status_t status;
		parameter_t parameter;
		void initialize();
	};

	struct communication_gesture_info_internal_t
	{
		std::uint8_t communication_slot[16];
		std::uint8_t communication_type[16];
		std::uint8_t gesture_slot[16];

		bool parse(json::value& data);
		void to_json(json::value& data) const;
		void initialize();
	};

	struct map_unlock_list_t
	{
		std::uint8_t map[2738];

		void to_json(json::value& value) const;
		bool parse_diff(json::value& value);
	};

	struct mission_record_list_internal_t
	{
		struct entry_t
		{
			std::uint8_t clear_flag;
			//std::uint8_t clear_rank;
			//std::uint8_t new_flag;
			std::uint16_t mission_code;
			//std::uint32_t clear_time;
			//std::uint32_t score;
		};

		std::uint8_t open[16];
		std::uint8_t cleared[16];

		bool open_mission(const std::uint32_t mission_code);
		bool parse_diff_single(json::value& data);
		bool parse_diff(json::value& data);
		void to_json(json::value& data) const;
	};

	struct quest_record_list_internal_t
	{
		struct entry_t
		{
			std::uint8_t flagset;
			std::uint8_t repop_count;
			std::uint32_t mission_code;
		};

		struct quest_t
		{
			std::uint8_t open : 1;
			std::uint8_t flagset : 7;
			std::uint8_t repop_count : 8;
		};

		quest_t list[180];

		bool open_mission(const std::uint32_t mission_code);
		bool parse_diff(json::value& data);
		void to_json(json::value& data) const;
	};

	struct replay_info_list_internal_t
	{
		struct entry_t
		{
			std::uint8_t difficulty;
			std::uint8_t is_clear;
			std::uint32_t mission_code;
		};

		std::uint8_t difficulty[26];
		std::uint8_t is_clear[4];

		bool parse_diff_single(json::value& data);
		void to_json(json::value& data) const;
	};
#pragma pack(pop)

	static_assert(sizeof(battle_pack_t) == 8);
	static_assert(sizeof(inventory_resource_t) == 8);
	static_assert(sizeof(stackable_item_t) == 8);

	using mission_record_list_t = database_struct<mission_record_list_internal_t>;
	using quest_record_list_t = database_struct<quest_record_list_internal_t>;
	using replay_info_list_t = database_struct<replay_info_list_internal_t>;

	using avatar_t = database_struct<avatar_internal_t>;
	using gimmick_info_t = database_struct<gimmick_info_internal_t>;
	using crew_levels_t = database_struct<crew_levels_internal_t>;
	using defense_mission_info_t = database_struct<defense_mission_info_internal_t>;
	using communication_gesture_info_t = database_struct<communication_gesture_info_internal_t>;

	constexpr const auto max_item_count = 99999u;

	enum inventory_type_t : std::uint8_t
	{
		inventory_storage = 0,
		inventory_player = 1,
		inventory_type_count = 2,
	};

	constexpr const auto max_nonstackable_items = 1024;
	class nonstackable_item_list_t final : public generic_item_list<nonstackable_item_t, max_nonstackable_items>
	{
	public:
		inline bool are_elements_equal(const nonstackable_item_t& l, const nonstackable_item_t& r) const override
		{
			return l.inventory_index == r.inventory_index;
		}

		inline bool is_element_empty(const nonstackable_item_t& value) const override
		{
			return value.production_id == 0;
		}

		bool parse_life_diff(json::value& data);
		bool find_free_index(std::uint16_t& index, std::uint32_t& obtain_order);
		nonstackable_item_t* find_at_index(const std::uint16_t inventory_index);
		bool add_item(nonstackable_item_t& item);

	};

	constexpr const auto max_stackable_items = 1024;
	class stackable_item_list_t final : public generic_item_list<stackable_item_t, max_stackable_items>
	{
	public:
		inline bool are_elements_equal(const stackable_item_t& l, const stackable_item_t& r) const override
		{
			return l.inventory_index == r.inventory_index && l.inventory_type == r.inventory_type;
		}

		inline bool is_element_empty(const stackable_item_t& value) const override
		{
			return value.production_index == 0 || value.count == 0;
		}

		stackable_item_t* find_item(const std::uint32_t production_id);
		stackable_item_t* get_entry(const std::uint16_t inventory_index, const std::uint8_t inventory_type);
		bool find_free_index(const std::uint8_t inventory_type, std::uint16_t& index, std::uint16_t& obtain_order) const;

		inline void import_element(stackable_item_t& dest, const stackable_item_t& src) const override
		{
			std::memcpy(&dest, &src, sizeof(stackable_item_t));
		}

		bool add_item(stackable_item_t& item, const std::uint8_t inventory_type = inventory_storage);

	};

	constexpr const auto max_resources = 512;
	class inventory_resource_list_t final : public generic_item_list<inventory_resource_t, max_resources>
	{
	public:
		inline bool are_elements_equal(const inventory_resource_t& l, const inventory_resource_t& r) const override
		{
			return l.inventory_index == r.inventory_index && l.inventory_type == r.inventory_type;
		}

		inline bool is_element_empty(const inventory_resource_t& value) const override
		{
			return value.resource_index == 0 || value.count == 0;
		}

		bool find_free_index(const std::uint8_t inventory_type, std::uint16_t& index, std::uint16_t& obtain_order) const;
		inventory_resource_t* get_entry(const std::uint16_t inventory_index, const std::uint8_t inventory_type);

		inline void import_element(inventory_resource_t& dest, const inventory_resource_t& src) const override
		{
			std::memcpy(&dest, &src, sizeof(inventory_resource_t));
		}

		bool add_resource(inventory_resource_t& resource, const std::uint8_t inventory_type = inventory_storage);
		bool spend_resource(const std::uint32_t id, std::uint32_t& count);

	};

	constexpr const auto max_battle_packs = 64;
	class battle_pack_list_t final : public generic_item_list<battle_pack_t, max_battle_packs>
	{
	public:
		inline bool are_elements_equal(const battle_pack_t& l, const battle_pack_t& r) const override
		{
			return l.inventory_index == r.inventory_index && l.inventory_type == r.inventory_type;
		}

		inline bool is_element_empty(const battle_pack_t& value) const override
		{
			return value.bp_index == 0 || value.count == 0;
		}

		battle_pack_t* find_item(const std::uint32_t production_id);
		battle_pack_t* get_entry(const std::uint16_t inventory_index, const std::uint8_t inventory_type);
		bool find_free_index(const std::uint8_t inventory_type, std::uint16_t& index, std::uint16_t& obtain_order) const;

		inline void import_element(battle_pack_t& dest, const battle_pack_t& src) const override
		{
			std::memcpy(&dest, &src, sizeof(battle_pack_t));
		}

		bool add_item(battle_pack_t& item, const std::uint8_t inventory_type = inventory_storage);

	};

	bool craft_recipe(const game::recipe_t& recipe, const std::uint32_t amount, inventory_resource_list_t& resource_list, stackable_item_list_t& stackable_item_list, player_inventory_t& inventory_info);
	bool craft_recipe(const game::customize_option_t& recipe, const std::uint32_t amount, inventory_resource_list_t& resource_list, stackable_item_list_t& stackable_item_list, player_inventory_t& inventory_info);

	class player
	{
	public:
		DEFINE_FIELD(player_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(f_user_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(player_index, sqlpp::integer_unsigned);
		DEFINE_FIELD(player_creation_date, sqlpp::time_point);
		DEFINE_FIELD(point, sqlpp::integer_unsigned);
		DEFINE_FIELD(nameplate, sqlpp::integer_unsigned);
		DEFINE_FIELD(playtime, sqlpp::integer_unsigned);
		DEFINE_FIELD(current_loadout, sqlpp::integer_unsigned);
		DEFINE_FIELD(avatar, sqlpp::binary);
		DEFINE_FIELD(mission_info, sqlpp::binary);
		DEFINE_FIELD(loadout_list, sqlpp::binary);
		DEFINE_FIELD(player_inventory, sqlpp::binary);
		DEFINE_FIELD(nonstackable_item_list, sqlpp::binary);
		DEFINE_FIELD(gimmick_info, sqlpp::binary);
		DEFINE_FIELD(gimmick_data_afghan, sqlpp::binary);
		DEFINE_FIELD(gimmick_data_africa, sqlpp::binary);
		DEFINE_FIELD(player_play_record, sqlpp::binary);
		DEFINE_FIELD(base_resources, sqlpp::binary);
		DEFINE_FIELD(story_unlock_info, sqlpp::binary);
		DEFINE_FIELD(mission_record_list, sqlpp::binary);
		DEFINE_FIELD(quest_record_list, sqlpp::binary);
		DEFINE_FIELD(replay_info_list, sqlpp::binary);
		DEFINE_FIELD(inventory_resource_list, sqlpp::binary);
		DEFINE_FIELD(stackable_item_list, sqlpp::binary);
		DEFINE_FIELD(battle_pack_list, sqlpp::binary);
		DEFINE_FIELD(map_unlock_list_afghan, sqlpp::binary);
		DEFINE_FIELD(map_unlock_list_africa, sqlpp::binary);
		DEFINE_FIELD(building_info_afghan, sqlpp::binary);
		DEFINE_FIELD(crew_levels, sqlpp::binary);
		DEFINE_FIELD(defense_mission_info, sqlpp::binary);
		DEFINE_FIELD(communication_gesture_info, sqlpp::binary);
		DEFINE_TABLE(players, 
			player_id_field_t, 
			f_user_id_field_t, 
			player_index_field_t,
			player_creation_date_field_t,
			point_field_t, 
			nameplate_field_t, 
			playtime_field_t,
			current_loadout_field_t,
			avatar_field_t, 
			mission_info_field_t, 
			loadout_list_field_t,
			player_inventory_field_t,
			nonstackable_item_list_field_t,
			gimmick_info_field_t,
			gimmick_data_afghan_field_t,
			gimmick_data_africa_field_t,
			player_play_record_field_t,
			base_resources_field_t,
			story_unlock_info_field_t,
			mission_record_list_field_t,
			quest_record_list_field_t,
			replay_info_list_field_t,
			inventory_resource_list_field_t,
			stackable_item_list_field_t,
			battle_pack_list_field_t,
			map_unlock_list_afghan_field_t,
			map_unlock_list_africa_field_t,
			building_info_afghan_field_t,
			crew_levels_field_t,
			defense_mission_info_field_t,
			communication_gesture_info_field_t
		);

		inline static table_t table;

		template <typename ...Args>
		player(const sqlpp::result_row_t<Args...>& row)
		{
			this->player_id_ = row.player_id;
			this->account_id_ = row.account_id;
			this->user_id_ = row.f_user_id;
			this->index_ = row.player_index;
			this->playtime_ = static_cast<std::uint32_t>(row.playtime);
			this->nameplate_ = static_cast<std::uint32_t>(row.nameplate);
			this->point_ = static_cast<std::uint32_t>(row.point);
			this->current_loadout_ = static_cast<std::uint32_t>(row.current_loadout);
			this->creation_date_ = row.player_creation_date.value().time_since_epoch();
			this->avatar_.deserialize(row.avatar.value());
			this->gimmick_info_.deserialize(row.gimmick_info.value());
			this->crew_levels_.deserialize(row.crew_levels.value());
			this->defense_mission_info_.deserialize(row.defense_mission_info.value());
			this->communication_gesture_info_.deserialize(row.communication_gesture_info.value());
			this->mission_record_list_.deserialize(row.mission_record_list.value());
			this->quest_record_list_.deserialize(row.quest_record_list.value());
			this->replay_info_list_.deserialize(row.replay_info_list.value());
		}

		GET_FIELD_H(std::uint64_t, player_id);
		GET_FIELD_H(std::uint64_t, user_id);
		GET_FIELD_H(std::uint64_t, account_id);
		GET_FIELD_H(std::uint64_t, index);
		GET_FIELD_H(std::uint32_t, playtime);
		GET_FIELD_H(std::uint32_t, nameplate);
		GET_FIELD_H(std::uint32_t, point);
		GET_FIELD_H(std::uint32_t, current_loadout);
		GET_FIELD_H(avatar_t, avatar);
		GET_FIELD_H(gimmick_info_t, gimmick_info);
		GET_FIELD_H(crew_levels_t, crew_levels);
		GET_FIELD_H(defense_mission_info_t, defense_mission_info);
		GET_FIELD_H(communication_gesture_info_t, communication_gesture_info);
		GET_FIELD_H(mission_record_list_t, mission_record_list);
		GET_FIELD_H(quest_record_list_t, quest_record_list);
		GET_FIELD_H(replay_info_list_t, replay_info_list);
		GET_FIELD_H(std::chrono::microseconds, creation_date);

		std::string get_name() const;
		std::uint64_t get_id() const;

		void get_loadout_list(loadout_list_t& loadout) const;
		void get_mission_info(mission_info_t& mission_info) const;
		void get_inventory(player_inventory_t& inventory) const;
		void get_gimmick_save_data(gimmick_save_data_t& gimmick_data, const std::uint32_t map_location) const;
		void get_play_record(player_play_record_t& play_record) const;
		void get_base_resources(base_resources_t& base_resources) const;
		void get_story_unlock_info(story_unlock_info_t& story_unlock_info) const;
		void get_building_info(building_info_t& building) const;
		void get_map_unlock_list_afghan(map_unlock_list_t& map_unlock_list) const;
		void get_map_unlock_list_africa(map_unlock_list_t& map_unlock_list) const;

		bool set_avatar(avatar_t& avatar) const;
		bool set_loadout_list(loadout_list_t& loadout) const;
		bool set_mission_info(mission_info_t& mission_info) const;
		bool set_inventory(player_inventory_t& inventory) const;
		bool set_gimmick_info(gimmick_info_t& gimmick_info) const;
		bool set_gimmick_save_data(gimmick_save_data_t& gimmick_data, const std::uint32_t map) const;
		bool set_play_record(player_play_record_t& play_record) const;
		bool set_base_resources(base_resources_t& base_resources) const;
		bool set_story_unlock_info(story_unlock_info_t& story_unlock_info) const;
		bool set_building_info(building_info_t& building) const;
		bool set_crew_levels(crew_levels_t& crew_levels) const;
		bool set_defense_mission_info(defense_mission_info_t& defense_mission) const;
		bool set_communication_gesture_info(communication_gesture_info_t& communication_gesture_info) const;
		bool set_map_unlock_list_afghan(map_unlock_list_t& map_unlock_list) const;
		bool set_map_unlock_list_africa(map_unlock_list_t& map_unlock_list) const;
		bool set_mission_record_list(mission_record_list_t& set_mission_record_list) const;
		bool set_quest_record_list(quest_record_list_t& quest_record_list) const;
		bool set_replay_info_list(replay_info_list_t& replay_info_list) const;

		void get_nonstackable_item_list(nonstackable_item_list_t& nonstackable_list, const std::size_t size_add = 0ull) const;
		void get_inventory_resource_list(inventory_resource_list_t& inventory_resource_list, const std::size_t size_add = 0ull) const;
		void get_stackable_item_list(stackable_item_list_t& stackable_item_list, const std::size_t size_add = 0ull) const;
		void get_battle_pack_list(battle_pack_list_t& stackable_item_list, const std::size_t size_add = 0ull) const;

		bool set_nonstackable_item_list(nonstackable_item_list_t& nonstackable_list) const;
		bool set_inventory_resource_list(inventory_resource_list_t& inventory_resource_list) const;
		bool set_stackable_item_list(stackable_item_list_t& stackable_item_list) const;
		bool set_battle_pack_list(battle_pack_list_t& stackable_item_list) const;

		void set_nameplate(const std::uint16_t nameplate) const;

	};

	std::optional<player> find(const std::uint64_t id);
	std::optional<player> find_by_index(const std::uint64_t user_id, const std::uint64_t player_index);
	std::vector<player> get_player_list(const std::uint64_t user_id);
	std::size_t get_player_count(const std::uint64_t user_id);
	std::optional<player> create(const std::uint64_t user_id);

	void initialize(const std::uint64_t player_id);
	void delete_player_data(const std::uint64_t player_id);
}
