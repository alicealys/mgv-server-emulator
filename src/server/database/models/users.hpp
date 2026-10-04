#pragma once

#include "../database.hpp"
#include "players.hpp"

namespace database::users
{
	struct bgm_settings_t
	{
		struct playlist_t
		{
			struct parseable
			{
				std::array<std::uint8_t, 32> cassette_ids;
				std::uint8_t index;
				std::string name;
			};

			std::uint8_t cassette_ids[32];
			char name[24];
		};

		std::uint8_t setting;
		playlist_t playlists[5];

		bool parse(json::value& data);
		void to_json(json::value& data) const;
	};

	struct user_inventory_t
	{
		std::uint8_t archive_new[32];
		std::uint8_t archive_obtained[32];
		std::uint8_t battle_pack_opened[128];
		std::uint8_t cassette_new[64];
		std::uint8_t cassette_obtained[64];
		std::uint8_t command_marker_new[16];
		std::uint8_t command_marker_obtained[16];
		std::uint8_t face_paint_new[8];
		std::uint8_t face_paint_obtained[8];
		std::uint8_t food_used[128];
		std::uint8_t gesture_new[16];
		std::uint8_t gesture_obtained[16];
		std::uint8_t name_plate_new[32];
		std::uint8_t name_plate_obtained[32];
		std::uint8_t preset_radio_new[32];
		std::uint8_t preset_radio_obtained[32]; // all FF
		std::uint8_t production_opened[256];
		std::uint8_t recipe_new[256];
		std::uint8_t recipe_new_for_db[256];
		std::uint8_t recipe_opened[256];
		std::uint8_t recipe_used[256];
		std::uint8_t resource_opened[128];
		bgm_settings_t bgm_settings;

		void initialize();
		bool parse_save(json::value& data);
		bool parse(json::value& data);
		void to_json(json::value& data);
		void open_production(const std::uint32_t index);

		template <typename T>
		void set_obtained_generic(T& type, const std::uint32_t index)
		{
			if (index >= sizeof(type) * 8)
			{
				return;
			}

			const auto byte_index = (index >> 3);
			type[byte_index] |= (1 << (index & 7));
		}

		void set_obtained_gesture(const std::uint32_t index);
		void set_obtained_radio(const std::uint32_t index);
		void set_obtained_marker(const std::uint32_t index);
		void set_obtained_nameplate(const std::uint32_t index);
		void set_obtained_face_paint(const std::uint32_t index);
		void set_obtained_battle_pack(const std::uint32_t index);
		void set_obtained_recipe(const std::uint32_t index);
		void set_obtained_cassette(const std::uint32_t index);
	};

	struct give_item_params_t
	{
		players::stackable_item_list_t* stackable_list;
		players::nonstackable_item_list_t* nonstackable_list;
		players::inventory_resource_list_t* resource_list;
		players::player_inventory_t* player_inventory;
		user_inventory_t* user_inventory;
		std::uint64_t text_id;
	};

	class user
	{
	public:
		DEFINE_FIELD(user_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(account_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(session_id, sqlpp::text);
		DEFINE_FIELD(current_player_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(password_hash, sqlpp::text);
		DEFINE_FIELD(crypto_key, sqlpp::text);
		DEFINE_FIELD(ex_ip, sqlpp::text);
		DEFINE_FIELD(ex_port, sqlpp::integer_unsigned);
		DEFINE_FIELD(in_ip, sqlpp::text);
		DEFINE_FIELD(in_port, sqlpp::integer_unsigned);
		DEFINE_FIELD(nat, sqlpp::integer_unsigned);
		DEFINE_FIELD(last_update, sqlpp::time_point);
		DEFINE_FIELD(user_creation_date, sqlpp::time_point);
		DEFINE_FIELD(last_daily_reward, sqlpp::time_point);
		DEFINE_FIELD(user_flag, sqlpp::integer_unsigned);
		DEFINE_FIELD(dlc_flag, sqlpp::integer_unsigned);
		DEFINE_FIELD(sv_coin, sqlpp::integer_unsigned);
		DEFINE_FIELD(loadout_count, sqlpp::integer_unsigned);
		DEFINE_FIELD(player_capacity, sqlpp::integer_unsigned);
		DEFINE_FIELD(user_inventory, sqlpp::binary);
		DEFINE_TABLE(users,
			user_id_field_t, 
			account_id_field_t, 
			session_id_field_t, 
			current_player_id_field_t, 
			password_hash_field_t, 
			crypto_key_field_t,
			ex_ip_field_t, 
			ex_port_field_t, 
			in_ip_field_t, 
			in_port_field_t, 
			nat_field_t,
			last_update_field_t, 
			user_creation_date_field_t,
			last_daily_reward_field_t,
			user_flag_field_t, 
			dlc_flag_field_t,
			sv_coin_field_t, 
			loadout_count_field_t,
			player_capacity_field_t,
			user_inventory_field_t
		);

		inline static table_t table;

		template <typename ...Args>
		user(const sqlpp::result_row_t<Args...>& row)
		{
			this->user_id_ = row.user_id;
			this->account_id_ = row.account_id;
			this->session_id_ = row.session_id;
			this->current_player_id_ = row.current_player_id;
			this->password_hash_ = row.password_hash;
			this->crypto_key_ = row.crypto_key;
			this->ex_ip_ = row.ex_ip;
			this->ex_port_ = static_cast<std::uint16_t>(row.ex_port);
			this->in_ip_ = row.in_ip;
			this->in_port_ = static_cast<std::uint16_t>(row.in_port);
			this->nat_ = static_cast<std::uint32_t>(row.nat);
			this->last_update_ = row.last_update.value().time_since_epoch();
			this->creation_date_ = std::chrono::duration_cast<std::chrono::seconds>(row.user_creation_date.value().time_since_epoch());
			this->last_daily_reward_ = std::chrono::duration_cast<std::chrono::seconds>(row.last_daily_reward.value().time_since_epoch());
			this->user_flag_ = static_cast<std::uint32_t>(row.user_flag);
			this->dlc_flag_ = static_cast<std::uint32_t>(row.dlc_flag);
			this->sv_coin_ = static_cast<std::uint32_t>(row.sv_coin);
			this->loadout_count_ = static_cast<std::uint32_t>(row.loadout_count);
			this->player_capacity_ = row.player_capacity;

			if (!row.player_id.is_null())
			{
				this->current_player.emplace(row);
			}
		}

		GET_FIELD_H(std::uint64_t, user_id);
		GET_FIELD_H(std::uint64_t, account_id);
		GET_FIELD_H(std::uint64_t, current_player_id);
		GET_FIELD_H(std::string, session_id);
		GET_FIELD_H(std::string, password_hash);
		GET_FIELD_H(std::string, crypto_key);
		GET_FIELD_H(std::string, ex_ip);
		GET_FIELD_H(std::string, in_ip);
		GET_FIELD_H(std::uint16_t, ex_port);
		GET_FIELD_H(std::uint16_t, in_port);
		GET_FIELD_H(std::uint32_t, user_flag);
		GET_FIELD_H(std::uint32_t, dlc_flag);
		GET_FIELD_H(std::uint32_t, sv_coin);
		GET_FIELD_H(std::uint32_t, loadout_count);
		GET_FIELD_H(std::uint64_t, player_capacity);
		GET_FIELD_H(std::chrono::microseconds, last_update);
		GET_FIELD_H(std::chrono::seconds, creation_date);
		GET_FIELD_H(std::chrono::seconds, last_daily_reward);

		std::string get_nat() const;
		std::uint64_t get_id() const;

		void get_inventory(user_inventory_t& inventory) const;

		bool set_inventory(user_inventory_t& inventory) const;

		void set_user_flag(const std::uint32_t flag) const;
		void set_dlc_flag(const std::uint32_t flag) const;

		bool spend_sv_coins(const std::uint32_t value) const;
		bool add_sv_coins(const std::uint32_t value) const;

		bool set_loadout_count(const std::uint32_t loadout_count) const;
		bool inc_player_capacity() const;
		bool set_daily_reward() const;

		bool give_item(const game::item_t& reward, give_item_params_t& params, json::value& reward_info) const;

		std::optional<players::player> create_additional_player() const;

		std::optional<players::player> current_player;

	private:
		std::uint32_t nat_{};

	};


	std::optional<user> find(const std::uint64_t user_id);
	bool exists(const std::uint64_t user_id);
	std::optional<user> find_from_account(const std::uint64_t account_id);
	std::optional<user> find_from_session_id(const std::string session_id, bool use_timeout, bool* is_expired = nullptr);
	user find_or_insert(const std::uint64_t account_id);

	bool generate_password(const std::uint64_t user_id, std::string& password);
	bool generate_session(const std::uint64_t user_id, std::string& session_id, std::string& crypto_key);

	void set_ip_and_port(const std::uint64_t user_id, const std::string& ex_ip, const std::uint16_t ex_port,
		const std::string& in_ip, const std::uint16_t in_port, const std::string& nat_type);

	bool update_session(const std::uint64_t user_id);

	bool set_current_player(const std::uint64_t user_id, const std::uint64_t player_id);
	bool reset_current_player(const std::uint64_t user_id);

	std::uint64_t get_user_count();
	std::uint64_t get_online_user_count();
	std::uint64_t get_online_user_count(const std::chrono::milliseconds within);

	bool add_sv_coins(const std::uint64_t user_id, const std::uint32_t value);
	std::uint32_t get_sv_coins(const std::uint64_t user_id);

	bool delete_all_user_data(const std::uint64_t account_id);
}
