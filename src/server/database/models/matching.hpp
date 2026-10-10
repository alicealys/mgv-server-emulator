#pragma once

#include "../database.hpp"

namespace database::matching
{
	constexpr const auto max_room_members = 5u;

	struct create_param_t
	{
		struct data_t
		{
			std::string data;
		};

		std::uint8_t flag_attr;
		std::uint8_t max_slot;
		std::string password;
		std::uint8_t region_matching_level;
		data_t room_member_bin_attr_internal;
		std::array<data_t, 2> room_searchable_bin_attr_external;
		std::array<std::int64_t, 16> room_searchable_int_attr_external;
	};

	struct search_param_t
	{
		std::uint8_t flag_attr;
		std::uint8_t flag_filter;
		std::uint32_t max;
		std::uint32_t option;
		std::uint8_t region_matching_level;
		std::uint32_t start_index;
		std::array<std::int64_t, 16> int_attr_param;
	};

	struct set_data_external_param_t
	{
		struct data_t
		{
			std::string data;
		};

		std::array<data_t, 2> room_searchable_bin_attr_external;
		std::array<std::int64_t, 16> room_searchable_int_attr_external;
		std::uint8_t region_matching_level;
		std::uint64_t room_id;
	};

	struct set_data_internal_param_t
	{
		std::uint8_t flag_attr;
		std::uint8_t flag_filter;
		std::uint64_t room_id;
	};

	enum room_status_t : std::uint32_t
	{
		status_init = 0,
		status_mission_start = 1,
		status_mission_end_confirm = 2,
		status_mission_end = 3,
	};

	class matching_member
	{
	public:
		DEFINE_FIELD(member_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(f_room_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(f_player_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(member_data, sqlpp::text);
		DEFINE_FIELD(migration_sequence, sqlpp::integer_unsigned);
		DEFINE_FIELD(create_date, sqlpp::time_point);
		DEFINE_TABLE(matching_members,
			member_id_field_t,
			f_room_id_field_t,
			f_player_id_field_t,
			member_data_field_t,
			migration_sequence_field_t,
			create_date_field_t
		);

		inline static table_t table;

		template <typename ...Args>
		matching_member(const sqlpp::result_row_t<Args...>& row)
		{
			this->member_id_ = row.member_id;
			this->room_id_ = row.f_room_id;
			this->player_id_ = row.f_player_id;
			this->member_data_ = row.member_data;
			this->user_id_ = row.user_id;
			this->account_id_ = row.account_id;
			this->ex_ip_ = row.ex_ip.value();
			this->ex_port_ = static_cast<std::uint16_t>(row.ex_port);
			this->migration_sequence_ = static_cast<std::uint32_t>(row.migration_sequence);
			this->create_date_ = std::chrono::duration_cast<std::chrono::seconds>(row.create_date.value().time_since_epoch());
		}

		GET_FIELD_H(std::uint64_t, member_id);
		GET_FIELD_H(std::uint64_t, room_id);
		GET_FIELD_H(std::uint64_t, player_id);
		GET_FIELD_H(std::uint64_t, account_id);
		GET_FIELD_H(std::uint64_t, user_id);
		GET_FIELD_H(std::uint16_t, ex_port);
		GET_FIELD_H(std::uint32_t, migration_sequence);
		GET_FIELD_H(std::string, ex_ip);
		GET_FIELD_H(std::string, member_data);
		GET_FIELD_H(std::chrono::seconds, create_date);

		std::chrono::seconds get_past_time() const;
	};

	class matching_room
	{
	public:
		DEFINE_FIELD(room_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(owner_player_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(f_coop_mission_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(max_slot, sqlpp::integer_unsigned);
		DEFINE_FIELD(region_matching_level, sqlpp::integer_unsigned);
		DEFINE_FIELD(flag_attr, sqlpp::integer_unsigned);
		DEFINE_FIELD(flag_filter, sqlpp::integer_unsigned);
		DEFINE_FIELD(status, sqlpp::integer_unsigned);
		DEFINE_FIELD(reserve_num, sqlpp::integer_unsigned);
		DEFINE_FIELD(password, sqlpp::text);
		DEFINE_FIELD(int_attr_01, sqlpp::integer);
		DEFINE_FIELD(int_attr_02, sqlpp::integer);
		DEFINE_FIELD(int_attr_03, sqlpp::integer);
		DEFINE_FIELD(int_attr_04, sqlpp::integer);
		DEFINE_FIELD(int_attr_05, sqlpp::integer);
		DEFINE_FIELD(int_attr_06, sqlpp::integer);
		DEFINE_FIELD(int_attr_07, sqlpp::integer);
		DEFINE_FIELD(int_attr_08, sqlpp::integer);
		DEFINE_FIELD(int_attr_09, sqlpp::integer);
		DEFINE_FIELD(int_attr_10, sqlpp::integer);
		DEFINE_FIELD(int_attr_11, sqlpp::integer);
		DEFINE_FIELD(int_attr_12, sqlpp::integer);
		DEFINE_FIELD(int_attr_13, sqlpp::integer);
		DEFINE_FIELD(int_attr_14, sqlpp::integer);
		DEFINE_FIELD(int_attr_15, sqlpp::integer);
		DEFINE_FIELD(int_attr_16, sqlpp::integer);
		DEFINE_FIELD(bin_attr_01, sqlpp::text);
		DEFINE_FIELD(bin_attr_02, sqlpp::text);
		DEFINE_FIELD(create_date, sqlpp::time_point);
		DEFINE_TABLE(matching_rooms,
			room_id_field_t,
			f_coop_mission_id_field_t,
			owner_player_id_field_t,
			max_slot_field_t,
			region_matching_level_field_t,
			flag_attr_field_t,
			flag_filter_field_t,
			status_field_t,
			reserve_num_field_t,
			password_field_t,
			int_attr_01_field_t,
			int_attr_02_field_t,
			int_attr_03_field_t,
			int_attr_04_field_t,
			int_attr_05_field_t,
			int_attr_06_field_t,
			int_attr_07_field_t,
			int_attr_08_field_t,
			int_attr_09_field_t,
			int_attr_10_field_t,
			int_attr_11_field_t,
			int_attr_12_field_t,
			int_attr_13_field_t,
			int_attr_14_field_t,
			int_attr_15_field_t,
			int_attr_16_field_t,
			bin_attr_01_field_t,
			bin_attr_02_field_t,
			create_date_field_t
		);

		inline static table_t table;

		template <typename ...Args>
		matching_room(const sqlpp::result_row_t<Args...>& row)
		{
			this->room_id_ = row.room_id;
			this->owner_player_id_ = row.owner_player_id;
			this->coop_mission_id_ = row.f_coop_mission_id;
			this->max_slot_ = static_cast<std::uint8_t>(row.max_slot);
			this->region_matching_level_ = static_cast<std::uint8_t>(row.region_matching_level);
			this->flag_attr_ = static_cast<std::uint8_t>(row.flag_attr);
			this->flag_filter_ = static_cast<std::uint8_t>(row.flag_filter);
			this->status_ = static_cast<std::uint32_t>(row.status);
			this->reserve_num_ = static_cast<std::uint8_t>(row.reserve_num);
			this->password_ = row.password;
			this->int_attr[0] = static_cast<std::uint32_t>(row.int_attr_01);
			this->int_attr[1] = static_cast<std::uint32_t>(row.int_attr_02);
			this->int_attr[2] = static_cast<std::uint32_t>(row.int_attr_03);
			this->int_attr[3] = static_cast<std::uint32_t>(row.int_attr_04);
			this->int_attr[4] = static_cast<std::uint32_t>(row.int_attr_05);
			this->int_attr[5] = static_cast<std::uint32_t>(row.int_attr_06);
			this->int_attr[6] = static_cast<std::uint32_t>(row.int_attr_07);
			this->int_attr[7] = static_cast<std::uint32_t>(row.int_attr_08);
			this->int_attr[8] = static_cast<std::uint32_t>(row.int_attr_09);
			this->int_attr[9] = static_cast<std::uint32_t>(row.int_attr_10);
			this->int_attr[10] = static_cast<std::uint32_t>(row.int_attr_11);
			this->int_attr[11] = static_cast<std::uint32_t>(row.int_attr_12);
			this->int_attr[12] = static_cast<std::uint32_t>(row.int_attr_13);
			this->int_attr[13] = static_cast<std::uint32_t>(row.int_attr_14);
			this->int_attr[14] = static_cast<std::uint32_t>(row.int_attr_15);
			this->int_attr[15] = static_cast<std::uint32_t>(row.int_attr_16);
			this->bin_attr[0] = row.bin_attr_01.value();
			this->bin_attr[1] = row.bin_attr_02.value();
			this->create_date_ = std::chrono::duration_cast<std::chrono::seconds>(row.create_date.value().time_since_epoch());
		}

		GET_FIELD_H(std::uint64_t, room_id);
		GET_FIELD_H(std::uint64_t, owner_player_id);
		GET_FIELD_H(std::uint64_t, coop_mission_id);
		GET_FIELD_H(std::uint8_t, max_slot);
		GET_FIELD_H(std::uint8_t, region_matching_level);
		GET_FIELD_H(std::uint8_t, flag_attr);
		GET_FIELD_H(std::uint8_t, flag_filter);
		GET_FIELD_H(std::uint32_t, status);
		GET_FIELD_H(std::uint8_t, reserve_num);
		GET_FIELD_H(std::string, password);
		GET_FIELD_H(std::int32_t, int_attr_01);
		GET_FIELD_H(std::int32_t, int_attr_02);
		GET_FIELD_H(std::int32_t, int_attr_03);
		GET_FIELD_H(std::int32_t, int_attr_04);
		GET_FIELD_H(std::int32_t, int_attr_05);
		GET_FIELD_H(std::int32_t, int_attr_06);
		GET_FIELD_H(std::int32_t, int_attr_07);
		GET_FIELD_H(std::int32_t, int_attr_08);
		GET_FIELD_H(std::int32_t, int_attr_09);
		GET_FIELD_H(std::int32_t, int_attr_10);
		GET_FIELD_H(std::int32_t, int_attr_11);
		GET_FIELD_H(std::int32_t, int_attr_12);
		GET_FIELD_H(std::int32_t, int_attr_13);
		GET_FIELD_H(std::int32_t, int_attr_14);
		GET_FIELD_H(std::int32_t, int_attr_15);
		GET_FIELD_H(std::int32_t, int_attr_16);
		GET_FIELD_H(std::chrono::seconds, create_date);

		std::array<std::uint32_t, 16> int_attr{};
		std::array<std::string, 2> bin_attr{};

	};

	std::uint64_t create_room(const std::uint64_t owner_id, const create_param_t& param);
	std::optional<matching_room> get_room(const std::uint64_t room_id);
	std::optional<matching_room> get_room_from_member(const std::uint64_t player_id);
	std::vector<matching_room> search_rooms(const std::uint64_t player_id, const search_param_t& param);

	std::vector<matching_member> get_members(const std::uint64_t room_id);
	void remove_member(const std::uint64_t room_id, const std::uint64_t player_id);
	void remove_member(const std::uint64_t player_id);
	std::uint64_t add_member(const std::uint64_t room_id, const std::uint64_t player_id, const std::string& data = {});
	void set_member_data(const std::uint64_t player_id, const std::string& data);
	void set_member_migration_sequence(const std::uint64_t player_id, const std::uint32_t migration_sequence);

	void update_room(const std::uint64_t room_id, const set_data_external_param_t& param);
	void update_room(const std::uint64_t room_id, const set_data_internal_param_t& param);

	void close_room(const std::uint64_t room_id);
	void check_close_room(const std::uint64_t room_id);
	void close_room_from_owner(const std::uint64_t owner_id);
	bool migrate_room_owner(const std::uint64_t room_id, const std::uint64_t owner_id);
	void set_room_owner(const std::uint64_t room_id, const std::uint64_t owner_id);
	void set_room_coop_mission_id(const std::uint64_t room_id, const std::uint64_t coop_mission_id);
	void set_room_status(const std::uint64_t room_id, const std::uint32_t status);
	void set_room_reserve_num(const std::uint64_t room_id, const std::uint8_t num);

	std::size_t get_total_room_count();
	std::size_t get_total_member_count();

	void delete_player_data(const std::uint64_t player_id);
}
