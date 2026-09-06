#pragma once

#include "../database.hpp"

namespace database::players
{
	constexpr const auto player_capacity = 1;

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

		void to_json(nlohmann::json& data);
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
		std::uint32_t arm_inventory_index;
		std::uint32_t body_inventory_index;
		std::uint32_t ext_head_production_idx;
		std::uint32_t ext_suit_production_idx;
		std::uint32_t head_inventory_index;
		std::uint32_t leg_inventory_index;
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
		std::uint32_t production_id;
		nonstackable_option_t option_list[8];
		perk_t perk_list[5];
		std::uint8_t reserved[256];
	};

	struct loadout_t
	{
		std::uint16_t index;
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

		void to_json(nlohmann::json& data);
	};

	struct mission_info_t
	{
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
		std::uint8_t vars[640]; // 
		std::uint8_t reserved[256];
	};

	struct inventory_t
	{
		std::uint32_t energy;
		std::uint32_t oxygen_convert_count;
		std::uint32_t energy_invested[5];
		std::uint16_t cbox_history[3];
		std::uint16_t cbox_location[3];
		std::uint16_t cbox_updated[3];
		std::int32_t cbox_pos[12];
		std::uint8_t reserved[256];
	};

	struct nonstackable_list_t
	{
		nonstackbable_t list[4];
		void to_json(nlohmann::json& data);
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
		DEFINE_FIELD(avatar, sqlpp::binary);
		DEFINE_FIELD(mission_info, sqlpp::binary);
		DEFINE_FIELD(loadout, sqlpp::binary);
		DEFINE_FIELD(inventory, sqlpp::binary);
		DEFINE_FIELD(nonstackable_list, sqlpp::binary);
		DEFINE_TABLE(players, player_id_field_t, f_user_id_field_t, player_index_field_t,
			player_creation_date_field_t,
			point_field_t, nameplate_field_t, playtime_field_t,
			avatar_field_t, 
			mission_info_field_t, 
			loadout_field_t,
			inventory_field_t,
			nonstackable_list_field_t
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
			this->creation_date_ = row.player_creation_date.value().time_since_epoch();
		}

		GET_FIELD_H(std::uint64_t, player_id);
		GET_FIELD_H(std::uint64_t, user_id);
		GET_FIELD_H(std::uint64_t, account_id);
		GET_FIELD_H(std::uint64_t, index);
		GET_FIELD_H(std::uint32_t, playtime);
		GET_FIELD_H(std::uint32_t, nameplate);
		GET_FIELD_H(std::uint32_t, point);
		GET_FIELD_H(std::chrono::microseconds, creation_date);

		std::string get_name() const;
		std::uint64_t get_id() const;

		void get_avatar(avatar_t& avatar) const;
		void get_loadout(loadout_t& loadout) const;
		void get_mission_info(mission_info_t& mission_info) const;
		void get_inventory(inventory_t& inventory) const;
		void get_nonstackable_list(nonstackable_list_t& nonstackable_list) const;

		bool set_avatar(avatar_t& avatar) const;
		bool set_loadout(loadout_t& loadout) const;
		bool set_mission_info(mission_info_t& mission_info) const;
		bool set_inventory(inventory_t& inventory) const;
		bool set_nonstackable_list(nonstackable_list_t& nonstackable_list) const;

	};

	std::optional<player> find(const std::uint64_t id);
	std::optional<player> find_by_index(const std::uint64_t user_id, const std::uint64_t player_index);
	std::vector<player> get_player_list(const std::uint64_t user_id);
	std::optional<player> create(const std::uint64_t user_id);

	void delete_player_data(const std::uint64_t player_id);

	void get_avatar(const std::uint64_t player_id, avatar_t& avatar);
	void get_loadout(const std::uint64_t player_id, loadout_t& loadout);
	void get_mission_info(const std::uint64_t player_id, mission_info_t& mission_info);
	void get_inventory(const std::uint64_t player_id, inventory_t& inventory);
	void get_nonstackable_list(const std::uint64_t player_id, nonstackable_list_t& nonstackable_list);
	bool set_avatar(const std::uint64_t player_id, avatar_t& avatar);
	bool set_loadout(const std::uint64_t player_id, loadout_t& loadout);
	bool set_mission_info(const std::uint64_t player_id, mission_info_t& mission_info);
	bool set_inventory(const std::uint64_t player_id, inventory_t& inventory);
	bool set_nonstackable_list(const std::uint64_t player_id, nonstackable_list_t& nonstackable_list);
}
