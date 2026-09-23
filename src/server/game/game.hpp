#pragma once

enum server_error
{
	NOERR,
	ERR_DEFENSE_MISSION_ALREADY_STARTED,
	ERR_DEFENSE_MISSION_NOT_IN_MISSION,
	ERR_PURCHASE_LIMIT,
	ERR_NOT_DEPLOY,
	ERR_ALREADY_DEPLOY,
	ERR_INACTIVE,
	ERR_SHORTAGE,
	ERR_ALREADY_ENABLED,
	ERR_NOT_FOUND,
	ERR_EXPIRED,
	ERR_RESTRICT_USER,
	ERR_COOP_ITEM_USED,
	ERR_COOP_ITEM_NOT_IN_MISSION,
	ERR_CREW_NOT_FOUND,
	ERR_BASE_RESOURCE_SHORTAGE,
	ERR_BUILDING_OVERLAPPED,
	ERR_BUILDING_MATERIAL_SHORTAGE,
	ERR_BUILDING_DOES_NOT_MEET_CONDITIONS,
	ERR_CRAFT_MATERIAL_SHORTAGE,
	ERR_CRAFT_DOES_NOT_MEET_CONDITIONS,
	ERR_PERMISSION_DENIED,
	ERR_ALREADY_LOCKED,
	ERR_NOT_IN_ROOM,
	ERR_ROOM_NOT_FOUND,
	ERR_PASSWORD_DO_NOT_MATCH,
	ERR_ROOMTOOMANY,
	ERR_ALREADYINROOM,
	ERR_NOT_SOLD,
	ERR_MAINTENANCE,
	ERR_BANNED,
	ERR_GMP_FULL,
	ERR_GMP_SHORTAGE,
	ERR_RESOURCE_FULL,
	ERR_RESOURCE_SHORTAGE,
	ERR_ALREADY_EXCHANGED,
	ERR_DETAIL_NOTFOUND,
	ERR_NOT_COMPLETE,
	ERR_ALREADY_COMPLETED,
	ERR_MBCOIN_SHORTAGE,
	ERR_PAYMENT_INCONSISTENCY,
	ERR_OVER_CAPACITY,
	ERR_TARGETPLAYER_NOTFOUND,
	ERR_NOTEMPTY,
	ERR_PLAYER_COUNT_FULL,
	ERR_PLAYER_NOTFOUND,
	ERR_AUTH_LIMIT,
	ERR_AUTHKONAMIID_SSLETC,
	ERR_AUTHKONAMIID_AUTH,
	ERR_LOGIN_FAILED,
	ERR_INVALID_SESSION,
	ERR_INVALID_TICKET,
	ERR_PREPARE_CIPHERINFO,
	ERR_READ_CIPHERINFO,
	ERR_READ_PASSPHRASE,
	ERR_GET_TICKETINFO,
	ERR_DEFCLIENTVER,
	ERR_ALREADYLOGGEDIN,
	ERR_ALREADY_EXISTS,
	ERR_INVALID_ACCOUNT,
	ERR_INVALIDARG,
	ERR_DATABASE,
	ERR_FLOWID_OUTOFRANGE,
	ERR_UNSELECTED_USE_FLOW,
	ERR_RESULT_ILLEGAL_FLOW,
	ERR_OPTION_ILLEGAL_FLOW,
	ERR_RESULT_OUTOFRANGE,
	ERR_OPTION_OUTOFRANGE,
	ERR_UNKNOWN,
	ERR_TIMEOUT,
	ERR_COUNT,
};

namespace game
{
	// server

	std::string get_error(const std::uint32_t error);
	const std::unordered_map<std::uint32_t, std::string>& get_error_map();

	enum static_key_t
	{
		key_type_ssd = 0,
		key_type_tpp = 1,
	};

	std::uint8_t* get_static_key(const std::uint32_t type = key_type_ssd);
	std::size_t get_static_key_len();

	extern std::unordered_set<std::uint32_t> mission_list;
	extern std::unordered_set<std::uint32_t> quest_list;

	enum reward_category_t
	{
		REWARD_RESOURCE = 0,
		REWARD_PRODUCTION = 1,
		REWARD_RECIPE = 2,
		REWARD_PRESET_RADIO = 3,
		REWARD_GESTURE = 4,
		REWARD_COMMUNICATION_MARKER = 5,
		REWARD_NAMEPLATE = 6,
		REWARD_PRIVILEGE = 7,
		REWARD_COIN = 8,
		REWARD_ENERGY = 9,
		REWARD_BATTLE_PACK = 10,
		REWARD_FACE_PAINT = 11,
		REWARD_CASSETTE = 12,
		REWARD_CHARACTER_SLOT = 13,
		REWARD_COUNT = 14,
	};

	struct reward_t
	{
		std::uint8_t category;
		std::uint32_t code;
		std::uint32_t param1;
		std::uint32_t param2;
		std::uint32_t param3;
		std::uint32_t param4;
		std::uint32_t param5;
		std::uint32_t num;

		bool parse(json::value& data);
		void to_json(json::value& data) const;
	};

	struct cost_t
	{
		std::uint32_t id;
		std::uint32_t type;
		std::uint32_t count;
	};

	struct gradeup_spec_t
	{
		bool parse(json::value& data);

		std::uint8_t rarity;
		std::uint16_t atk;
		std::uint16_t w_life;
		std::uint16_t def;
		std::uint16_t a_life;
	};

	struct survival_gear_t
	{
		bool parse(json::value& data);

		std::uint32_t id;
		std::uint32_t index;
		std::uint32_t type_id;
		std::uint32_t target_id;
		std::uint32_t value_u1;
		std::uint32_t mesh;
	};

	struct customize_option_t
	{
		bool parse(json::value& data);

		std::string id_str;
		std::uint32_t id;
		std::uint32_t type_id;
		std::uint32_t related_id1;
		std::uint32_t related_id2;
		std::uint32_t value_u1;
		float value_f1;
		std::uint32_t price;
		cost_t cost[4];
		std::uint64_t lang_name;
		std::uint64_t lang_name2;
		std::uint64_t lang_name3;
		std::uint64_t lang_info;
		std::uint64_t icon_path;
	};

	struct customize_option_group_t
	{
		bool parse(json::value& data);

		struct option_t
		{
			std::uint32_t optid;
			bool obtained;
			std::uint8_t level;
			std::shared_ptr<customize_option_t> option;
		};

		std::string id_str;
		std::uint32_t id;
		bool not_empty;
		std::array<option_t, 8> options;
	};

	struct customize_t
	{
		bool parse(json::value& data);

		std::string id_str;
		std::uint32_t id;
		std::uint32_t option_slots_min;
		std::array<std::uint32_t, 8> option_slot_ids;
		std::array<std::shared_ptr<customize_option_group_t>, 8> option_slots;
	};

	struct resource_t
	{
		bool parse(json::value& data);

		std::string id_str;
		std::uint32_t id;
		std::uint32_t index;
		std::uint32_t type;
		std::uint32_t material;
		std::uint8_t rarity;
		float weight;
		std::uint32_t repop;
		bool in_kakin_tab;
		bool enable_takeout;
		std::uint32_t conv_home;
		std::uint32_t conv_use;
		std::uint32_t base_c_water;
		std::uint32_t base_food;
		std::uint32_t base_medicine;
		std::uint32_t sup_combat;
		std::uint32_t sup_survival;
		float crew_inj_food_poison;
		float crew_inj_dysentery;
		float crew_inj_physical;
		float crew_inj_fatigue;
		std::uint32_t related_id;
		std::uint64_t lang_name;
		std::uint64_t lang_name2;
		std::uint64_t lang_name3;
		std::uint64_t lang_info;
		bool show_info;
		std::uint64_t icon_path;
	};

	struct production_t
	{
		bool parse(json::value& data);
		std::shared_ptr<survival_gear_t> get_survival_gear();

		std::uint32_t id;
		std::uint32_t index;
		std::uint32_t type;
		std::uint32_t craft_category;
		std::uint32_t rarity;
		std::uint32_t lv_need;
		std::uint32_t stock;
		std::uint32_t life;
		std::uint32_t customize_id;
		std::uint32_t conv_use;
		std::uint32_t base_c_water;
		std::uint32_t base_food;
		std::uint32_t base_medicine;
		std::uint32_t sup_combat;
		std::uint32_t sup_survival;
		std::uint32_t related_id;
		std::uint32_t attr_icon;
		std::uint32_t material;
		bool only_flag;
		bool countable;
		bool eqp_hip;
		bool eqp_back;
		bool eqp_sec;
		bool eqp_sup;
		bool pri_r_slot;
		bool show_info;
		float weight;
		float crew_inj_food_poison;
		float crew_inj_dysentery;
		float crew_inj_physical;
		float crew_inj_fatigue;
		float prv_r_x;
		float prv_r_y;
		float prv_r_y_min;
		float prv_r_y_max;
		float prv_r_z_off;
		float prv_zm_min;
		float prv_zm;
		float prv_zm_max;
		std::uint64_t lang_name;
		std::uint64_t lang_name2;
		std::uint64_t lang_name3;
		std::uint64_t lang_info;
		std::uint64_t icon_path;
		std::array<std::uint32_t, 3> perk;
		std::string id_str;
		std::shared_ptr<customize_t> customize;
	};

	struct recipe_t
	{
		bool parse(json::value& data);

		struct leftover_t
		{
			std::uint32_t id;
			std::uint32_t type;
			std::uint32_t count;
		};

		bool opened;
		bool junk;
		std::uint32_t index;
		std::uint32_t group;
		std::uint32_t dev_level;
		std::uint32_t count;
		std::uint32_t trans_time;
		std::uint32_t price;
		std::uint32_t potential_id;
		std::uint32_t id;
		std::string id_str;
		leftover_t leftover[1];
		cost_t cost[6];
		std::array<std::uint32_t, 2> multi_craft;
		std::uint32_t production_id;

		std::shared_ptr<production_t> production;
	};

	struct crew_member_type_t
	{
		bool parse(json::value& data);

        std::uint16_t face;
        std::uint16_t race;
        std::uint16_t sex;
        std::uint16_t body;
        std::uint16_t develop;
        std::uint16_t food;
        std::uint16_t medic;
        std::uint16_t farm;
        std::uint16_t base_defense;
        std::uint16_t combat_deploy;
        std::uint16_t thirst_resist;
        std::uint16_t hunger_resist;
        std::uint16_t injury_resist;
        std::uint16_t sick_resist;
        std::uint16_t sleeplack_resist;
        std::uint16_t life;
        std::uint16_t job_table;
		std::uint16_t skill_table;
		std::uint32_t id;
		std::string id_str;
	};

	struct defense_mission_settings_t
	{
		bool parse(json::value& data);

		struct ui_reward_info_t
		{
			std::uint8_t rarity;
			std::uint32_t icon_type;
		};

		struct rank_reward_info_t
		{
			std::array<ui_reward_info_t, 8> line_info;
			std::string rank;
		};

		std::vector<float> wave_time;
		std::vector<std::uint32_t> interval;
		std::array<std::uint32_t, 5> rank_threshold;
		std::uint32_t mission_type;
		std::uint16_t mission_id;
		std::uint16_t max_wave_count;
		std::array<rank_reward_info_t, 6> rank_reward_info;
	};

	std::int16_t calc_gradeup_life(const std::uint32_t base_life, const std::uint32_t grade);

	bool is_event_obtained_res(const std::uint32_t resource_id, std::uint8_t* obtained, bool* result);
}
