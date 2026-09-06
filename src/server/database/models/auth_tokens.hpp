#pragma once

#include "../database.hpp"

namespace database::auth_tokens
{
	enum auth_token_type_t
	{
		token_game_auth = 0,
		token_web_auth = 1,
		token_type_count = 2,
	};

	class auth_token
	{
	public:
		DEFINE_FIELD(id, sqlpp::integer_unsigned);
		DEFINE_FIELD(type, sqlpp::integer_unsigned);
		DEFINE_FIELD(account_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(token_hash, sqlpp::text);
		DEFINE_FIELD(expire_date, sqlpp::time_point);
		DEFINE_TABLE(steam_users, id_field_t, account_id_field_t, 
			type_field_t, token_hash_field_t, expire_date_field_t);

		inline static table_t table;

		template <typename ...Args>
		auth_token(const sqlpp::result_row_t<Args...>& row)
		{
			this->id_ = row.id;
			this->type_ = static_cast<std::uint32_t>(row.type);
			this->account_id_ = row.account_id;
			this->token_hash_ = row.token_hash;
			this->expire_date_ = row.expire_date;
		}

		GET_FIELD_H(std::uint64_t, id);
		GET_FIELD_H(std::uint64_t, account_id);
		GET_FIELD_H(std::uint32_t, type);
		GET_FIELD_H(std::string, token_hash);
		GET_FIELD_H(std::chrono::system_clock::time_point, expire_date);
	};

	bool generate_auth_token(const std::uint32_t type, const std::uint64_t account_id, std::string& auth_token);
	bool authenticate(const std::uint32_t type, const std::uint64_t account_id, const std::string& token);
}
