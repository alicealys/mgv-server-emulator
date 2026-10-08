#pragma once

#include "../database.hpp"

namespace database::matching
{
	constexpr const auto max_room_members = 4u;

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
		std::array<std::uint32_t, 16> room_searchable_int_attr_external;
	};

	struct search_param_t
	{
		std::uint8_t flag_attr;
		std::uint8_t flag_filter;
		std::uint32_t max;
		std::uint32_t option;
		std::uint8_t region_matching_level;
		std::uint32_t start_index;
		std::array<std::uint32_t, 16> room_searchable_int_attr_external;
	};

	struct set_data_external_param_t
	{
		struct data_t
		{
			std::string data;
		};

		std::array<data_t, 2> room_searchable_bin_attr_external;
		std::array<std::uint32_t, 16> room_searchable_int_attr_external;
		std::uint8_t region_matching_level;
		std::uint64_t room_id;
	};

	struct set_data_internal_param_t
	{
		std::uint8_t flag_attr;
		std::uint8_t flag_filter;
		std::uint64_t room_id;
	};

	class matching_member
	{
	public:
		DEFINE_FIELD(member_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(f_room_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(f_player_id, sqlpp::integer_unsigned);
		DEFINE_TABLE(matching_members,
			member_id_field_t,
			f_room_id_field_t,
			f_player_id_field_t
		);

		inline static table_t table;

		template <typename ...Args>
		matching_member(const sqlpp::result_row_t<Args...>& row)
		{
			this->member_id_ = row.member_id;
			this->room_id_ = row.f_room_id;
			this->player_id_ = row.f_player_id;
		}

		GET_FIELD_H(std::uint64_t, member_id);
		GET_FIELD_H(std::uint64_t, room_id);
		GET_FIELD_H(std::uint64_t, player_id);
	};

	class matching_room
	{
	public:
		DEFINE_FIELD(room_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(owner_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(max_slot, sqlpp::integer_unsigned);
		DEFINE_FIELD(region_matching_level, sqlpp::integer_unsigned);
		DEFINE_FIELD(flag_attr, sqlpp::integer_unsigned);
		DEFINE_FIELD(flag_filter, sqlpp::integer_unsigned);
		DEFINE_FIELD(int_attr_01, sqlpp::integer_unsigned);
		DEFINE_FIELD(int_attr_02, sqlpp::integer_unsigned);
		DEFINE_FIELD(int_attr_03, sqlpp::integer_unsigned);
		DEFINE_FIELD(int_attr_04, sqlpp::integer_unsigned);
		DEFINE_FIELD(int_attr_05, sqlpp::integer_unsigned);
		DEFINE_FIELD(int_attr_06, sqlpp::integer_unsigned);
		DEFINE_FIELD(int_attr_07, sqlpp::integer_unsigned);
		DEFINE_FIELD(int_attr_08, sqlpp::integer_unsigned);
		DEFINE_FIELD(int_attr_09, sqlpp::integer_unsigned);
		DEFINE_FIELD(int_attr_10, sqlpp::integer_unsigned);
		DEFINE_FIELD(int_attr_11, sqlpp::integer_unsigned);
		DEFINE_FIELD(int_attr_12, sqlpp::integer_unsigned);
		DEFINE_FIELD(int_attr_13, sqlpp::integer_unsigned);
		DEFINE_FIELD(int_attr_14, sqlpp::integer_unsigned);
		DEFINE_FIELD(int_attr_15, sqlpp::integer_unsigned);
		DEFINE_FIELD(int_attr_16, sqlpp::integer_unsigned);
		DEFINE_FIELD(password, sqlpp::text);
		DEFINE_FIELD(create_date, sqlpp::time_point);
		DEFINE_TABLE(matching_rooms,
			room_id_field_t,
			owner_id_field_t,
			max_slot_field_t,
			region_matching_level_field_t,
			flag_attr_field_t,
			flag_filter_field_t,
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
			password_field_t,
			create_date_field_t
		);

		inline static table_t table;

		template <typename ...Args>
		matching_room(const sqlpp::result_row_t<Args...>& row)
		{
			this->room_id_ = row.room_id;
			this->owner_id_ = row.owner_id;
			this->max_slot_ = static_cast<std::uint8_t>(row.max_slot);
			this->region_matching_level_ = static_cast<std::uint8_t>(row.region_matching_level);
			this->flag_attr_ = static_cast<std::uint8_t>(row.flag_attr);
			this->flag_filter_ = static_cast<std::uint8_t>(row.flag_filter);
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
			this->create_date_ = std::chrono::duration_cast<std::chrono::seconds>(row.create_date.value().time_since_epoch());
		}

		GET_FIELD_H(std::uint64_t, room_id);
		GET_FIELD_H(std::uint64_t, owner_id);
		GET_FIELD_H(std::uint8_t, max_slot);
		GET_FIELD_H(std::uint8_t, region_matching_level);
		GET_FIELD_H(std::uint8_t, flag_attr);
		GET_FIELD_H(std::uint8_t, flag_filter);
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

	};

	std::uint64_t create_room(const std::uint64_t owner_id, const create_param_t& param);
	std::optional<matching_room> get_room(const std::uint64_t room_id);
	std::optional<matching_room> get_room_from_member(const std::uint64_t player_id);
	std::vector<matching_room> search_rooms(const std::uint64_t player_id, const search_param_t& param);

	std::vector<matching_member> get_members(const std::uint64_t room_id);
	void remove_member(const std::uint64_t room_id, const std::uint64_t player_id);
	void remove_member(const std::uint64_t player_id);
	std::uint64_t add_member(const std::uint64_t room_id, const std::uint64_t player_id);

	void update_room(const std::uint64_t room_id, const set_data_external_param_t& param);
	void update_room(const std::uint64_t room_id, const set_data_internal_param_t& param);
}
