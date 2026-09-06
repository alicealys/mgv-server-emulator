#include <std_include.hpp>

#include "auth_tokens.hpp"
#include "../auth.hpp"

#include <utils/cryptography.hpp>
#include <utils/string.hpp>

namespace database::auth_tokens
{
	namespace
	{
		std::chrono::system_clock::duration get_token_duration(const std::uint32_t type)
		{
			switch (type)
			{
			case token_game_auth:
				return 365 * 24h;
			case token_web_auth:
				return 7 * 24h;
			}

			return 24h;
		}
	}

	GET_FIELD_C(auth_token, std::uint64_t, id);
	GET_FIELD_C(auth_token, std::uint64_t, account_id);
	GET_FIELD_C(auth_token, std::uint32_t, type);
	GET_FIELD_C(auth_token, std::string, token_hash);

	namespace impl
	{
		template <database_type_t Type>
		bool generate_auth_token(const std::uint32_t type, const std::uint64_t account_id, std::string& auth_token)
		{
			auth_token = auth::generate_data(16, false);
			const auto hash = utils::cryptography::sha256::compute(auth_token, true);
			const auto duration = get_token_duration(type);

			return database::access<bool>([&](database_t& db)
			{
				const auto exists = db.get_database<Type>()->operator()(
					sqlpp::select(auth_token::table.id)
						.from(auth_token::table)
							.where(auth_token::table.type == type && auth_token::table.account_id == account_id));

				if (exists.empty())
				{
					auto results = db.get_database<Type>()->operator()(
						sqlpp::insert_into(auth_token::table)
							.set(auth_token::table.account_id = account_id,
								 auth_token::table.type = type,
								 auth_token::table.token_hash = hash,
								 auth_token::table.expire_date = std::chrono::system_clock::now() + duration
							));
					return results != 0u;
				}
				else
				{
					auto results = db.get_database<Type>()->operator()(
						sqlpp::update(auth_token::table)
							.set(auth_token::table.token_hash = hash,
							     auth_token::table.type = type,
								 auth_token::table.expire_date = std::chrono::system_clock::now() + duration)
									.where(auth_token::table.account_id == account_id)
						);
					return results != 0u;
				}
			});
		}

		template <database_type_t Type>
		bool authenticate(const std::uint32_t type, const std::uint64_t account_id, const std::string& auth_token)
		{
			const auto hash = utils::cryptography::sha256::compute(auth_token, true);
			return database::access<bool>([&](database_t& db)
			{
				auto results = db.get_database<Type>()->operator()(
					sqlpp::select(auth_token::table.account_id)
						.from(auth_token::table)
							.where(auth_token::table.type == type &&
								   auth_token::table.account_id == account_id &&
								   auth_token::table.token_hash == hash &&
								   auth_token::table.expire_date > std::chrono::system_clock::now()
						));
				return !results.empty();
			});
			return false;
		}

		template <database_type_t Type>
		void delete_user_data(const std::uint64_t account_id)
		{
			return database::access([&](database_t& db)
			{
				db.get_database<Type>()->operator()(
					sqlpp::remove_from(auth_token::table)
						.where(auth_token::table.account_id == account_id));
			});
		}
	}

	bool generate_auth_token(const std::uint32_t type, const std::uint64_t account_id, std::string& auth_token)
	{
		RUN_IMPL(impl::generate_auth_token, type, account_id, auth_token);
	}

	bool authenticate(const std::uint32_t type, const std::uint64_t account_id, const std::string& auth_token)
	{
		RUN_IMPL(impl::authenticate, type, account_id, auth_token);
	}

	void delete_user_data(const std::uint64_t account_id)
	{
		RUN_IMPL(impl::delete_user_data, account_id);
	}

	class table final : public table_interface
	{
	public:
		void create(database_t& database) override
		{
			database.run_query("mgvdb.auth_tokens.create");
		}
	};
}

REGISTER_TABLE(database::auth_tokens::table, 1)
