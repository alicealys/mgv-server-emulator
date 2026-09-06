#include <std_include.hpp>

#include "players.hpp"
#include "users.hpp"
#include "variables.hpp"
#include "../auth.hpp"

#include <utils/cryptography.hpp>
#include <utils/string.hpp>

namespace database::users
{
	namespace
	{
		bool is_session_expired(const user& user)
		{
			const auto now = std::chrono::duration_cast<std::chrono::microseconds>(
				std::chrono::system_clock::now().time_since_epoch());
			return (now - user.get_last_update()) > database::vars.session_timeout;
		}

#define IS_ACTIVE_EXPR user::table.last_update >= std::chrono::system_clock::now() - database::vars.session_timeout
	}

	GET_FIELD_C(user, std::uint64_t, id);
	GET_FIELD_C(user, std::uint64_t, account_id);
	GET_FIELD_C(user, std::uint64_t, current_player_id);
	GET_FIELD_C(user, std::string, session_id);
	GET_FIELD_C(user, std::string, password_hash);
	GET_FIELD_C(user, std::string, crypto_key);
	GET_FIELD_C(user, std::string, currency);
	GET_FIELD_C(user, std::chrono::microseconds, last_update);
	GET_FIELD_C(user, std::chrono::microseconds, creation_time);

	namespace impl
	{
		template <database_type_t Type>
		std::optional<user> find(const std::uint64_t user_id)
		{
			return database::access<std::optional<user>>([&](database::database_t& db)
				-> std::optional<user>
			{
				auto results = db.get_database<Type>()->operator()(
					sqlpp::select(
						sqlpp::all_of(user::table))
							.from(user::table)
								.where(user::table.id == user_id));

				if (results.empty())
				{
					return {};
				}

				const auto& row = results.front();
				return {user(row)};
			});
		}

		template <database_type_t Type>
		bool exists(const std::uint64_t user_id)
		{
			return database::access<bool>([&](database::database_t& db)
			{
				auto results = db.get_database<Type>()->operator()(
					sqlpp::select(
						sqlpp::count(1))
							.from(user::table)
								.where(user::table.id == user_id));

				return results.front().count.value() > 0;
			});
		}

		template <database_type_t Type>
		std::optional<user> find_from_account(const std::uint64_t account_id)
		{
			return database::access<std::optional<user>>([&](database::database_t& db)
				-> std::optional<user>
			{
				auto results = db.get_database<Type>()->operator()(
					sqlpp::select(
						sqlpp::all_of(user::table))
							.from(user::table)
								.where(user::table.account_id == account_id));

				if (results.empty())
				{
					return {};
				}

				const auto& row = results.front();
				return {user(row)};
			});
		}

		template <database_type_t Type>
		std::optional<user> find_by_session_id(const std::string session_id, bool use_timeout, bool* is_expired)
		{
			return database::access<std::optional<user>>([&](database::database_t& db)
				-> std::optional<user>
			{
				auto results = db.get_database<Type>()->operator()(
					sqlpp::select(
						sqlpp::all_of(user::table))
							.from(user::table)
								.where(user::table.session_id == session_id));

				if (results.empty())
				{
					return {};
				}

				const auto& row = results.front();
				user player(row);

				const auto expired = is_session_expired(player);
				if (is_expired != nullptr)
				{
					*is_expired = expired;
				}

				if (use_timeout && expired)
				{
					return {};
				}

				return {player};
			});
		}

		template <database_type_t Type>
		user find_or_insert(const std::uint64_t account_id)
		{
			{
				const auto found = find_from_account<Type>(account_id);
				if (found.has_value())
				{
					return found.value();
				}
			}

			database::access([&](database::database_t& db)
			{
				db.get_database<Type>()->operator()(
					sqlpp::insert_into(user::table)
						.set(user::table.account_id = account_id,
							 user::table.currency = "EUR",
							 user::table.last_update = std::chrono::system_clock::now(),
							 user::table.creation_time = std::chrono::system_clock::now()));
			});

			const auto found = find_from_account<Type>(account_id);
			if (!found.has_value())
			{
				throw std::runtime_error("[database::users::insert] Insertion failed");
			}

			return found.value();
		}

		template <database_type_t Type>
		void delete_user_data(const std::uint64_t user_id)
		{
			return database::access([&](database_t& db)
			{
				db.get_database<Type>()->operator()(
					sqlpp::remove_from(user::table)
						.where(user::table.id == user_id));
			});
		}

		template <database_type_t Type>
		std::string generate_password(const std::uint64_t user_id)
		{
			const auto password = auth::generate_data(16, false);
			const auto hash = utils::cryptography::md5::compute(password);
			const auto hash_b64 = utils::cryptography::base64::encode(hash);

			database::access([&](database::database_t& db)
			{
				db.get_database<Type>()->operator()(
					sqlpp::update(user::table)
						.set(user::table.password_hash = hash_b64)
							.where(user::table.id == user_id));
			});

			return password;
		}

		template <database_type_t Type>
		std::string generate_session_id(const std::uint64_t user_id)
		{
			const auto session_id = auth::generate_data(16, false);

			database::access([&](database::database_t& db)
			{
				db.get_database<Type>()->operator()(
					sqlpp::update(user::table)
						.set(user::table.session_id = session_id,
							 user::table.last_update = std::chrono::system_clock::now())
								.where(user::table.id == user_id));
			});

			return session_id;
		}

		template <database_type_t Type>
		std::string generate_crypto_key(const std::uint64_t user_id)
		{
			const auto crypto_key = auth::generate_data(16, true);

			database::access([&](database::database_t& db)
			{
				db.get_database<Type>()->operator()(
					sqlpp::update(user::table)
						.set(user::table.crypto_key = crypto_key)
							.where(user::table.id == user_id));
			});

			return crypto_key;
		}

		template <database_type_t Type>
		bool update_session(const user& player)
		{
			size_t result = 0;
			if (!is_session_expired(player))
			{
				database::access([&](database::database_t& db)
				{
					result = db.get_database<Type>()->operator()(
						sqlpp::update(user::table)
							.set(user::table.last_update = std::chrono::system_clock::now())
								.where(user::table.id == player.get_id()));
				});
			}
			else
			{
				database::access([&](database::database_t& db)
				{
					result = db.get_database<Type>()->operator()(
						sqlpp::update(user::table)
							.set(user::table.last_update = std::chrono::system_clock::now())
									.where(user::table.id == player.get_id()));
				});
			}

			return result != 0;
		}

	}

	std::optional<user> find(const std::uint64_t user_id)
	{
		RUN_IMPL(impl::find, user_id);
	}

	bool exists(const std::uint64_t user_id)
	{
		RUN_IMPL(impl::exists, user_id);
	}

	std::optional<user> find_from_account(const std::uint64_t account_id)
	{
		RUN_IMPL(impl::find_from_account, account_id);
	}

	std::optional<user> find_by_session_id(const std::string session_id, bool use_timeout, bool* is_expired)
	{
		RUN_IMPL(impl::find_by_session_id, session_id, use_timeout, is_expired);
	}

	user find_or_insert(const std::uint64_t account_id)
	{
		RUN_IMPL(impl::find_or_insert, account_id);
	}

	std::string generate_password(const std::uint64_t account_id)
	{
		RUN_IMPL(impl::generate_password, account_id);
	}

	std::string generate_session_id(const std::uint64_t account_id)
	{
		RUN_IMPL(impl::generate_session_id, account_id);
	}

	std::string generate_crypto_key(const std::uint64_t account_id)
	{
		RUN_IMPL(impl::generate_crypto_key, account_id);
	}

	void delete_user_data(const std::uint64_t user_id)
	{
		RUN_IMPL(impl::delete_user_data, user_id);
	}

	bool delete_all_user_data(const std::uint64_t account_id)
	{
		const auto user = find(account_id);
		if (!user.has_value())
		{
			return false;
		}

		delete_user_data(user->get_id());

		const auto players = players::get_player_list(user->get_id());
		for (const auto& player : players)
		{
			players::delete_player_data(player.get_id());
		}

		return true;
	}

	class table final : public table_interface
	{
	public:
		void create(database_t& database) override
		{
			database.run_query("mgvdb.users.create");
		}
	};
}

REGISTER_TABLE(database::users::table, 2)
