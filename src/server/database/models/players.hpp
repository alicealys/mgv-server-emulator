#pragma once

#include "../database.hpp"

namespace database::players
{
	std::uint32_t get_nat_type_id(const std::string& nat_type);
	std::string get_nat_type(const std::uint32_t nat_type_id);

	class player
	{
	public:
		DEFINE_FIELD(id, sqlpp::integer_unsigned);
		DEFINE_FIELD(user_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(player_index, sqlpp::integer_unsigned);
		DEFINE_FIELD(ex_ip, sqlpp::text);
		DEFINE_FIELD(ex_port, sqlpp::integer_unsigned);
		DEFINE_FIELD(in_ip, sqlpp::text);
		DEFINE_FIELD(in_port, sqlpp::integer_unsigned);
		DEFINE_FIELD(nat, sqlpp::integer_unsigned);
		DEFINE_FIELD(creation_time, sqlpp::time_point);
		DEFINE_TABLE(players, id_field_t, user_id_field_t, player_index_field_t, ex_ip_field_t, ex_port_field_t,
			in_ip_field_t, in_port_field_t, nat_field_t, creation_time_field_t);

		inline static table_t table;

		template <typename ...Args>
		player(const sqlpp::result_row_t<Args...>& row)
		{
			this->id_ = row.id;
			this->account_id_ = row.account_id;
			this->user_id_ = row.user_id;
			this->index_ = row.player_index;
			this->ex_ip_ = row.ex_ip;
			this->ex_port_ = static_cast<std::uint16_t>(row.ex_port);
			this->in_ip_ = row.in_ip;
			this->in_port_ = static_cast<std::uint16_t>(row.in_port);
			this->nat_ = static_cast<std::uint32_t>(row.nat);
			this->creation_time_ = row.creation_time.value().time_since_epoch();
		}

		GET_FIELD_H(std::uint64_t, id);
		GET_FIELD_H(std::uint64_t, user_id);
		GET_FIELD_H(std::uint64_t, account_id);
		GET_FIELD_H(std::uint64_t, index);
		GET_FIELD_H(std::string, ex_ip);
		GET_FIELD_H(std::string, in_ip);
		GET_FIELD_H(std::uint16_t, ex_port);
		GET_FIELD_H(std::uint16_t, in_port);
		GET_FIELD_H(std::chrono::microseconds, last_update);
		GET_FIELD_H(std::chrono::microseconds, creation_time);

		std::string get_nat() const;
		std::string get_name() const;

	private:
		std::uint32_t nat_{};

	};

	std::optional<player> find(const std::uint64_t id);
	std::optional<player> find_by_index(const std::uint64_t user_id, const std::uint64_t player_index);
	std::vector<player> get_player_list(const std::uint64_t user_id);

	void set_ip_and_port(const std::uint64_t player_id, const std::string& ex_ip, const std::uint16_t ex_port,
		const std::string& in_ip, const std::uint16_t in_port, const std::string& nat_type);

	void delete_player_data(const std::uint64_t player_id);
}
