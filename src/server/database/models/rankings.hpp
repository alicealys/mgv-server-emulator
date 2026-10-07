#pragma once

#include "../database.hpp"
#include "users.hpp"

namespace database::rankings
{
	enum ranking_type_t : std::uint8_t
	{
		ranking_type_event,
		ranking_type_reserved2,
		ranking_type_reserved3,
		ranking_type_reserved4,
		ranking_type_reserved5,
		ranking_type_count,
	};

	enum ranking_lookup_type_t : std::uint8_t
	{
		lookup_best,
		lookup_around,
		lookup_grade
	};

	class ranking
	{
	public:
		DEFINE_FIELD(ranking_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(f_user_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(ranking_type, sqlpp::integer_unsigned);
		DEFINE_FIELD(ranking_state, sqlpp::integer_unsigned);
		DEFINE_FIELD(player_rank, sqlpp::integer_unsigned);
		DEFINE_FIELD(player_rank_number, sqlpp::integer_unsigned);
		DEFINE_FIELD(points, sqlpp::integer);
		DEFINE_TABLE(rankings, 
			ranking_id_field_t,
			f_user_id_field_t,
			ranking_type_field_t,
			ranking_state_field_t,
			player_rank_field_t,
			player_rank_number_field_t,
			points_field_t
		);

		inline static table_t table;

		template <typename ...Args>
		ranking(const sqlpp::result_row_t<Args...>& row)
		{
			this->ranking_id_ = row.ranking_id;
			this->user_id_ = row.f_user_id;
			this->ranking_type_ = static_cast<std::uint8_t>(row.ranking_type);
			this->ranking_state_ = static_cast<std::uint32_t>(row.ranking_state);
			this->rank_ = row.player_rank;
			this->rank_number_ = row.player_rank_number;
			this->points_ = static_cast<std::int32_t>(row.points);
			this->account_id_ = row.account_id;
			this->user_loadout_header_.deserialize(row.user_loadout_header.value());
		}

		GET_FIELD_H(std::uint64_t, ranking_id);
		GET_FIELD_H(std::uint64_t, user_id);
		GET_FIELD_H(std::uint8_t, ranking_type);
		GET_FIELD_H(std::uint32_t, ranking_state);
		GET_FIELD_H(std::uint64_t, rank);
		GET_FIELD_H(std::uint64_t, rank_number);
		GET_FIELD_H(std::int32_t, points);
		GET_FIELD_H(std::uint64_t, account_id);
		GET_FIELD_H(users::user_loadout_header_t, user_loadout_header);
	};

	void create_entries(const std::uint64_t user_id);

	bool set_points(const std::uint64_t user_id, const std::uint8_t ranking_type, const std::int32_t value);
	bool increment_points(const std::uint64_t user_id, const std::uint8_t ranking_type, const std::int32_t count);
	bool set_points_if_bigger(const std::uint64_t user_id, const std::uint8_t ranking_type, const std::int32_t value);

	std::optional<std::uint64_t> get_user_rank(const std::uint64_t user_id, const std::uint8_t ranking_type);
	std::optional<ranking> get_user_entry(const std::uint64_t user_id, const std::uint8_t ranking_type);
	std::vector<ranking> get_account_entries(const std::vector<std::uint64_t>& account_id, const std::uint8_t ranking_type);
	std::vector<ranking> get_entries(const std::uint8_t ranking_type, const std::uint64_t offset, const std::uint32_t num, const std::uint64_t min_rank = 1u);
	std::size_t get_entry_count(const std::uint8_t ranking_type);

	void reset_points(const std::uint8_t ranking_type);
	void update_rankings(const std::uint8_t ranking_type);

	std::vector<ranking> get_entries_with_state(const std::uint8_t ranking_type, const std::uint32_t state, const std::uint32_t num);
	void set_ranking_state(const std::uint64_t ranking_id, const std::uint32_t ranking_state);

	void delete_user_data(const std::uint64_t user_id);
}
