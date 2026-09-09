#pragma once

#include "../database.hpp"
#include "game/game.hpp"

namespace database::players
{
	constexpr const auto player_capacity = 1u;
	constexpr const auto max_loadout_count = 8u;
	constexpr const auto initial_loadout_count = 4u;

	struct avatar_t
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

		bool parse(nlohmann::json& data);
		void to_json(nlohmann::json& data) const;
	};

	struct avatar_status_t
	{
		std::uint16_t equipment_slot;
		std::uint16_t hunger;
		std::uint16_t injury_whole;
		std::uint16_t life;
		std::uint16_t oxygen;
		std::uint16_t sequence_number;
		std::uint16_t stamina;
		std::uint16_t story_sequence;
		std::uint16_t thirst;
		std::uint16_t tiredness;
		std::uint16_t injury_part[8];
	};

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

	struct nonstackable_option_t
	{
		std::uint8_t obtained;
		std::uint16_t option_id;
	};

	struct perk_t
	{
		std::uint16_t perk_id;
		std::uint16_t perk_level;
	};

	struct nonstackbable_t
	{
		std::uint16_t color;
		std::uint16_t color2;
		std::uint16_t flag;
		std::uint16_t grade;
		std::uint16_t inventory_index;
		std::uint16_t life;
		std::uint16_t life_max;
		std::uint16_t obtain_order;
		std::uint16_t option_slot;
		std::uint16_t spec;
		std::uint64_t production_id;
		nonstackable_option_t option_list[8];
		perk_t perk_list[5];
		std::uint8_t reserved[256];

		bool parse(nlohmann::json& data, const bool parse_arrays);
		void to_json(nlohmann::json& data) const;
	};

	struct stackable_item_t
	{
		std::uint16_t cbox_index;
		std::uint16_t count;
		std::uint16_t damaged_in_count;
		std::uint16_t flag;
		std::uint16_t inventory_index;
		std::uint16_t inventory_type;
		std::uint16_t obtain_order;
		std::uint64_t production_id;

		bool parse(nlohmann::json& data);
		void to_json(nlohmann::json& data) const;
	};

	struct loadout_t
	{
		bool valid;
		std::uint16_t class_info;
		gear_info_t gear_info;
		weapon_t main_weapon_list[3];
		weapon_t sub_weapon_list[3];
		skill_t skill_list[5];
		item_t gadget_list[16];
		std::uint16_t survival_list[16];
		item_t porch_list[20];
		char name[64];
		std::uint8_t reserved[256];

		void initialize(const std::uint32_t index = 0u);
		bool parse(nlohmann::json& data, std::uint32_t& index);
		void to_json(nlohmann::json& data, const std::uint32_t index = 0u) const;
	};

	struct mission_info_t
	{
		struct status_buffer_t
		{
			std::uint32_t buffer_type;
			std::uint32_t remaining_time;
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
		std::uint8_t reserved[256];

		void initialize();
		bool parse(nlohmann::json& data);
		void to_json(nlohmann::json& data) const;
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
		std::uint8_t reserved[256];

		void initialize();
		bool parse(nlohmann::json& data, std::uint16_t& nameplate);
		bool parse_save(nlohmann::json& data, std::uint16_t& nameplate);
		void to_json(nlohmann::json& data, const std::uint16_t nameplate = 0u) const;
	};

	struct loadout_list_t
	{
		loadout_t list[max_loadout_count];
	};

	struct nonstackable_list_t
	{
		nonstackbable_t list[4];
		void to_json(nlohmann::json& data) const;
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

		void to_json(nlohmann::json& data, const std::uint32_t map_location) const;
	};

	struct gimmick_resource_info_t
	{
		std::uint32_t resource_event_tail;
		std::uint32_t resource_normal_tail;
		std::uint32_t resource_rare_tail;
		std::uint32_t resource_shared_tail;

		bool parse(nlohmann::json& data);
		void to_json(nlohmann::json& data) const;
	};

	struct gimmick_timer_info_t
	{
		std::uint16_t resource_timer_global_afghan;
		std::uint16_t resource_timer_global_africa;
		std::uint16_t resource_timer_stock_afghan;
		std::uint16_t resource_timer_stock_africa;

		bool parse(nlohmann::json& data);
		void to_json(nlohmann::json& data) const;
	};

	struct gimmick_info_t
	{
		gimmick_resource_info_t resource_afghan;
		gimmick_resource_info_t resource_africa;
		gimmick_timer_info_t timer;
	};

	struct base_resources_t
	{
		struct base_resource_params_t
		{
			std::uint16_t bad_status_1_risk;
			std::uint16_t bad_status_2_risk;
			std::uint16_t bad_status_3_risk;
			std::uint16_t bad_status_4_risk;
			std::uint16_t clean_water;
			std::uint16_t dirty_water;
			std::uint16_t food;
			std::uint16_t medical_supplies;
			std::uint16_t party_item_id;
			std::uint16_t party_item_updates;
			std::uint16_t total_number_of_updates;
		};

		std::uint32_t animals[57];
		std::uint32_t resource_counts[32];
		std::uint64_t next_update_time;
		std::uint64_t update_remaining_time;
		base_resource_params_t params;

		bool parse_base(nlohmann::json& base);
		bool parse_counts(nlohmann::json& count);
		bool parse_animals(nlohmann::json& animals);
		void to_json(nlohmann::json& data) const;
	};

	struct inventory_resource_t
	{
		std::uint16_t inventory_index;
		std::uint16_t inventory_type;
		std::uint32_t cbox_index;
		std::uint32_t count;
		std::uint32_t damaged_in_count;
		std::uint32_t flag;
		std::uint32_t obtain_order;
		std::uint32_t resource_id;
	};

	struct inventory_resources_t
	{
		inventory_resource_t resources[255];
	};

	struct player_play_record_t
	{
		std::uint32_t first[191];
		std::uint32_t additional[46];
	};

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
		DEFINE_FIELD(loadout_count, sqlpp::integer_unsigned);
		DEFINE_FIELD(avatar, sqlpp::binary);
		DEFINE_FIELD(mission_info, sqlpp::binary);
		DEFINE_FIELD(loadout_list, sqlpp::binary);
		DEFINE_FIELD(inventory, sqlpp::binary);
		DEFINE_FIELD(nonstackable_list, sqlpp::binary);
		DEFINE_FIELD(gimmick_info, sqlpp::binary);
		DEFINE_FIELD(gimmick_data_afghan, sqlpp::binary);
		DEFINE_FIELD(gimmick_data_africa, sqlpp::binary);
		DEFINE_FIELD(player_play_record, sqlpp::binary);
		DEFINE_FIELD(base_resources, sqlpp::binary);
		DEFINE_TABLE(players, player_id_field_t, f_user_id_field_t, player_index_field_t,
			player_creation_date_field_t,
			point_field_t, nameplate_field_t, playtime_field_t,
			current_loadout_field_t,
			loadout_count_field_t,
			avatar_field_t, 
			mission_info_field_t, 
			loadout_list_field_t,
			inventory_field_t,
			nonstackable_list_field_t,
			gimmick_info_field_t,
			gimmick_data_afghan_field_t,
			gimmick_data_africa_field_t,
			player_play_record_field_t,
			base_resources_field_t
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
			this->loadout_count_ = static_cast<std::uint32_t>(row.loadout_count);
			this->creation_date_ = row.player_creation_date.value().time_since_epoch();
		}

		GET_FIELD_H(std::uint64_t, player_id);
		GET_FIELD_H(std::uint64_t, user_id);
		GET_FIELD_H(std::uint64_t, account_id);
		GET_FIELD_H(std::uint64_t, index);
		GET_FIELD_H(std::uint32_t, playtime);
		GET_FIELD_H(std::uint32_t, nameplate);
		GET_FIELD_H(std::uint32_t, point);
		GET_FIELD_H(std::uint32_t, current_loadout);
		GET_FIELD_H(std::uint32_t, loadout_count);
		GET_FIELD_H(std::chrono::microseconds, creation_date);

		std::string get_name() const;
		std::uint64_t get_id() const;

		void get_avatar(avatar_t& avatar) const;
		void get_loadout_list(loadout_list_t& loadout) const;
		void get_mission_info(mission_info_t& mission_info) const;
		void get_inventory(player_inventory_t& inventory) const;
		void get_nonstackable_list(nonstackable_list_t& nonstackable_list) const;
		void get_gimmick_info(gimmick_info_t& gimmick_info) const;
		void get_gimmick_save_data(gimmick_save_data_t& gimmick_data, const std::uint32_t map) const;
		void get_play_record(player_play_record_t& play_record) const;
		void get_base_resources(base_resources_t& base_resources) const;

		bool set_avatar(avatar_t& avatar) const;
		bool set_loadout_list(loadout_list_t& loadout) const;
		bool set_mission_info(mission_info_t& mission_info) const;
		bool set_inventory(player_inventory_t& inventory) const;
		bool set_nonstackable_list(nonstackable_list_t& nonstackable_list) const;
		bool set_gimmick_info(gimmick_info_t& gimmick_info) const;
		bool set_gimmick_save_data(gimmick_save_data_t& gimmick_data, const std::uint32_t map) const;
		bool set_play_record(player_play_record_t& play_record) const;
		bool set_base_resources(base_resources_t& base_resources) const;

		void set_nameplate(const std::uint16_t nameplate) const;

	};

	std::optional<player> find(const std::uint64_t id);
	std::optional<player> find_by_index(const std::uint64_t user_id, const std::uint64_t player_index);
	std::vector<player> get_player_list(const std::uint64_t user_id);
	std::optional<player> create(const std::uint64_t user_id);

	void delete_player_data(const std::uint64_t player_id);

	void get_avatar(const std::uint64_t player_id, avatar_t& avatar);
	void get_loadout_list(const std::uint64_t player_id, loadout_list_t& loadout_list);
	void get_mission_info(const std::uint64_t player_id, mission_info_t& mission_info);
	void get_inventory(const std::uint64_t player_id, player_inventory_t& inventory);
	void get_nonstackable_list(const std::uint64_t player_id, nonstackable_list_t& nonstackable_list);

	bool set_avatar(const std::uint64_t player_id, avatar_t& avatar);
	bool set_loadout_list(const std::uint64_t player_id, loadout_list_t& loadout_list);
	bool set_mission_info(const std::uint64_t player_id, mission_info_t& mission_info);
	bool set_inventory(const std::uint64_t player_id, player_inventory_t& inventory);
	bool set_nonstackable_list(const std::uint64_t player_id, nonstackable_list_t& nonstackable_list);
}
