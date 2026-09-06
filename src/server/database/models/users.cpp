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

		namespace
		{
			std::vector<std::string> nat_types =
			{
				"SYMMETRIC_NAT",
				"RESTRICTED_PORT_CONE_NAT",
				"RESTRICTED_CONE_NAT",
				"OPEN_CHOICE_PORT",
				"FULL_CONE_NAT",
				"SYMMETRIC_OPEN",
				"SYMMETRIC_UDP_FIREWALL",
				"OPEN_INTERNET"
			};
		}

		std::uint32_t get_nat_type_id(const std::string& nat_type)
		{
			for (auto i = 0u; i < nat_types.size(); i++)
			{
				if (nat_type == nat_types[i])
				{
					return i;
				}
			}

			return 0;
		}

		std::string get_nat_type(const std::uint32_t nat_type_id)
		{
			if (nat_type_id < nat_types.size())
			{
				return nat_types[nat_type_id];
			}

			return nat_types[0];
		}

		namespace exp
		{
			const auto select =
				sqlpp::select(sqlpp::all_of(user::table), sqlpp::all_of(players::player::table))
					.from(user::table.left_outer_join(players::player::table).on(user::table.user_id == players::player::table.f_user_id));
		}
	}

	GET_FIELD_C(user, std::uint64_t, user_id);
	GET_FIELD_C(user, std::uint64_t, account_id);
	GET_FIELD_C(user, std::uint64_t, current_player_id);
	GET_FIELD_C(user, std::string, session_id);
	GET_FIELD_C(user, std::string, password_hash);
	GET_FIELD_C(user, std::string, crypto_key);
	GET_FIELD_C(user, std::string, currency);
	GET_FIELD_C(user, std::string, ex_ip);
	GET_FIELD_C(user, std::string, in_ip);
	GET_FIELD_C(user, std::uint16_t, ex_port);
	GET_FIELD_C(user, std::uint16_t, in_port);
	GET_FIELD_C(user, std::chrono::microseconds, last_update);
	GET_FIELD_C(user, std::chrono::microseconds, creation_date);

	std::string user::get_nat() const
	{
		return get_nat_type(this->nat_);
	}

	std::uint64_t user::get_id() const
	{
		return this->user_id_;
	}

	namespace impl
	{
		template <database_type_t Type>
		std::optional<user> find(const std::uint64_t user_id)
		{
			return database::access<std::optional<user>>([&](database_t& db)
				-> std::optional<user>
			{
				auto results = db.exec<Type>(exp::select.where(user::table.user_id == user_id));

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
			return database::access<bool>([&](database_t& db)
			{
				auto results = db.exec<Type>(
					sqlpp::select(
						sqlpp::count(1))
							.from(user::table)
								.where(user::table.user_id == user_id));

				return results.front().count.value() > 0;
			});
		}

		template <database_type_t Type>
		std::optional<user> find_from_account(const std::uint64_t account_id)
		{
			return database::access<std::optional<user>>([&](database::database_t& db)
				-> std::optional<user>
			{
				auto results = db.exec<Type>(exp::select.where(user::table.account_id == account_id));

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
				auto results = db.exec<Type>(exp::select.where(user::table.session_id == session_id));

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
				db.exec<Type>(
					sqlpp::insert_into(user::table)
						.set(user::table.account_id = account_id,
							 user::table.currency = "EUR",
							 user::table.last_update = std::chrono::system_clock::now(),
							 user::table.user_creation_date = std::chrono::system_clock::now()));
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
				db.exec<Type>(
					sqlpp::remove_from(user::table)
						.where(user::table.user_id == user_id));
			});
		}

		template <database_type_t Type>
		bool generate_password(const std::uint64_t user_id, std::string& password)
		{
			password = auth::generate_data(16, false);
			const auto hash = utils::cryptography::md5::compute(password);
			const auto hash_b64 = utils::cryptography::base64::encode(hash);

			return database::access<bool>([&](database::database_t& db)
			{
				const auto result = db.exec<Type>(
					sqlpp::update(user::table)
						.set(user::table.password_hash = hash_b64)
							.where(user::table.user_id == user_id));
				return result != 0ull;
			});
		}

		template <database_type_t Type>
		bool generate_session(const std::uint64_t user_id, std::string& session_id, std::string& crypto_key)
		{
			session_id = auth::generate_data(16, false);
			crypto_key = auth::generate_data(16, true);

			return database::access<bool>([&](database::database_t& db)
			{
				const auto result = db.exec<Type>(
					sqlpp::update(user::table)
						.set(user::table.session_id = session_id,
							 user::table.crypto_key = crypto_key,
							 user::table.last_update = std::chrono::system_clock::now())
								.where(user::table.user_id == user_id));
				return result != 0ull;
			});
		}

		template <database_type_t Type>
		bool update_session(const user& user)
		{
			size_t result = 0;
			if (!is_session_expired(user))
			{
				database::access([&](database::database_t& db)
				{
					result = db.exec<Type>(
						sqlpp::update(user::table)
							.set(user::table.last_update = std::chrono::system_clock::now())
								.where(user::table.user_id == user.get_user_id()));
				});
			}
			else
			{
				database::access([&](database::database_t& db)
				{
					result = db.exec<Type>(
						sqlpp::update(user::table)
							.set(user::table.last_update = std::chrono::system_clock::now())
									.where(user::table.user_id == user.get_user_id()));
				});
			}

			return result != 0;
		}

		template <database_type_t Type>
		void set_ip_and_port(const std::uint64_t player_id, const std::string& ex_ip, const std::uint16_t ex_port,
			const std::string& in_ip, const std::uint16_t in_port, const std::string& nat_type)
		{
			const auto nat_type_id = get_nat_type_id(nat_type);

			database::access([&](database::database_t& db)
			{
				db.exec<Type>(
					sqlpp::update(user::table)
						.set(user::table.ex_ip = ex_ip,
							 user::table.in_ip = in_ip,
							 user::table.ex_port = ex_port,
							 user::table.in_port = in_port,
							 user::table.nat = nat_type_id)
								.where(user::table.user_id == player_id));
			});
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

	bool generate_password(const std::uint64_t user_id, std::string& password)
	{
		RUN_IMPL(impl::generate_password, user_id, password);
	}

	bool generate_session(const std::uint64_t user_id, std::string& session_id, std::string& crypto_key)
	{
		RUN_IMPL(impl::generate_session, user_id, session_id, crypto_key);
	}

	void set_ip_and_port(const std::uint64_t player_id, const std::string& ex_ip, const std::uint16_t ex_port,
		const std::string& in_ip, const std::uint16_t in_port, const std::string& nat_type)
	{
		RUN_IMPL(impl::set_ip_and_port, player_id, ex_ip, ex_port, in_ip, in_port, nat_type);
	}

	bool update_session(const user& user)
	{
		RUN_IMPL(impl::update_session, user);
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

		delete_user_data(user->get_user_id());

		const auto players = players::get_player_list(user->get_user_id());
		for (const auto& player : players)
		{
			players::delete_player_data(player.get_player_id());
		}

		return true;
	}

	class table final : public table_interface
	{
	public:
		void create(database_t& database) override
		{
			database.run_query("mgssd.users.create");
		}
	};
}

REGISTER_TABLE(database::users::table, 2)
