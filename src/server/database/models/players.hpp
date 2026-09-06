#pragma once

#include "../database.hpp"

namespace database::players
{
	class player
	{
	public:
		DEFINE_FIELD(player_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(f_user_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(player_index, sqlpp::integer_unsigned);
		DEFINE_FIELD(player_creation_date, sqlpp::time_point);
		DEFINE_TABLE(players, player_id_field_t, f_user_id_field_t, player_index_field_t,
			player_creation_date_field_t);

		inline static table_t table;

		template <typename ...Args>
		player(const sqlpp::result_row_t<Args...>& row)
		{
			this->player_id_ = row.player_id;
			this->account_id_ = row.account_id;
			this->user_id_ = row.f_user_id;
			this->index_ = row.player_index;
			this->creation_date_ = row.player_creation_date.value().time_since_epoch();
		}

		GET_FIELD_H(std::uint64_t, player_id);
		GET_FIELD_H(std::uint64_t, user_id);
		GET_FIELD_H(std::uint64_t, account_id);
		GET_FIELD_H(std::uint64_t, index);
		GET_FIELD_H(std::chrono::microseconds, last_update);
		GET_FIELD_H(std::chrono::microseconds, creation_date);

		std::string get_name() const;
		std::uint64_t get_id() const;

	};

	std::optional<player> find(const std::uint64_t id);
	std::optional<player> find_by_index(const std::uint64_t user_id, const std::uint64_t player_index);
	std::vector<player> get_player_list(const std::uint64_t user_id);

	void delete_player_data(const std::uint64_t player_id);
}
