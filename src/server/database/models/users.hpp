#pragma once

#include "../database.hpp"

namespace database::users
{
	class user
	{
	public:
		DEFINE_FIELD(id, sqlpp::integer_unsigned);
		DEFINE_FIELD(account_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(session_id, sqlpp::text);
		DEFINE_FIELD(current_player_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(password_hash, sqlpp::text);
		DEFINE_FIELD(crypto_key, sqlpp::text);
		DEFINE_FIELD(currency, sqlpp::text);
		DEFINE_FIELD(last_update, sqlpp::time_point);
		DEFINE_FIELD(creation_time, sqlpp::time_point);
		DEFINE_TABLE(users,
			id_field_t, 
			account_id_field_t, session_id_field_t, 
			current_player_id_field_t, 
			password_hash_field_t, crypto_key_field_t,
			currency_field_t,
			last_update_field_t, creation_time_field_t
		);

		inline static table_t table;

		template <typename ...Args>
		user(const sqlpp::result_row_t<Args...>& row)
		{
			this->id_ = row.id;
			this->account_id_ = row.account_id;
			this->session_id_ = row.session_id;
			this->current_player_id_ = row.current_player_id;
			this->password_hash_ = row.password_hash;
			this->crypto_key_ = row.crypto_key;
			this->currency_ = row.currency;
			this->last_update_ = row.last_update.value().time_since_epoch();
			this->creation_time_ = row.creation_time.value().time_since_epoch();
		}

		GET_FIELD_H(std::uint64_t, id);
		GET_FIELD_H(std::uint64_t, account_id);
		GET_FIELD_H(std::uint64_t, current_player_id);
		GET_FIELD_H(std::string, session_id);
		GET_FIELD_H(std::string, password_hash);
		GET_FIELD_H(std::string, crypto_key);
		GET_FIELD_H(std::string, currency);
		GET_FIELD_H(std::chrono::microseconds, last_update);
		GET_FIELD_H(std::chrono::microseconds, creation_time);
	};


	std::optional<user> find(const std::uint64_t user_id);
	bool exists(const std::uint64_t user_id);
	std::optional<user> find_from_account(const std::uint64_t account_id);
	std::optional<user> find_by_session_id(const std::string session_id, bool use_timeout, bool* is_expired = nullptr);
	user find_or_insert(const std::uint64_t account_id);

	std::string generate_password(const std::uint64_t user_id);
	std::string generate_session_id(const std::uint64_t user_id);
	std::string generate_crypto_key(const std::uint64_t user_id);

	bool delete_all_user_data(const std::uint64_t account_id);
}
