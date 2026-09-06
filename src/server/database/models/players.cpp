#include <std_include.hpp>

#include "players.hpp"
#include "users.hpp"
#include "../auth.hpp"

#include <utils/cryptography.hpp>
#include <utils/string.hpp>

namespace database::players
{
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

	GET_FIELD_C(player, std::uint64_t, id);
	GET_FIELD_C(player, std::uint64_t, user_id);
	GET_FIELD_C(player, std::uint64_t, account_id);
	GET_FIELD_C(player, std::uint64_t, index);
	GET_FIELD_C(player, std::string, ex_ip);
	GET_FIELD_C(player, std::string, in_ip);
	GET_FIELD_C(player, std::uint16_t, ex_port);
	GET_FIELD_C(player, std::uint16_t, in_port);
	GET_FIELD_C(player, std::chrono::microseconds, last_update);
	GET_FIELD_C(player, std::chrono::microseconds, creation_time);

	std::string player::get_nat() const
	{
		return get_nat_type(this->nat_);
	}

	std::string player::get_name() const
	{
		return std::format("{}_player{:02}", this->get_account_id(), this->get_index() + 1);
	}

	namespace impl
	{
		template <database_type_t Type>
		std::optional<player> find(const std::uint64_t player_id)
		{
			return database::access<std::optional<player>>([&](database::database_t& db)
				-> std::optional<player>
			{
				auto results = db.get_database<Type>()->operator()(
					sqlpp::select(
						sqlpp::all_of(player::table), users::user::table.account_id)
							.from(player::table.join(users::user::table).on(users::user::table.id == player::table.user_id))
								.where(player::table.id == player_id));

				if (results.empty())
				{
					return {};
				}

				const auto& row = results.front();
				return {player(row)};
			});
		}

		template <database_type_t Type>
		std::optional<player> find_by_index(const std::uint64_t user_id, const std::uint64_t player_index)
		{
			return database::access<std::optional<player>>([&](database::database_t& db)
				-> std::optional<player>
			{
				auto results = db.get_database<Type>()->operator()(
					sqlpp::select(
						sqlpp::all_of(player::table), users::user::table.account_id)
							.from(player::table.join(users::user::table).on(users::user::table.id == player::table.user_id))
								.where(player::table.user_id == user_id && player::table.player_index == player_index));

				if (results.empty())
				{
					return {};
				}

				const auto& row = results.front();
				return {player(row)};
			});
		}

		template <database_type_t Type>
		player create(const std::uint64_t user_id)
		{
			const auto id = database::access<std::uint64_t>([&](database::database_t& db)
			{
				return db.get_database<Type>()->operator()(
					sqlpp::insert_into(player::table)
						.set(player::table.user_id = user_id,
							 player::table.creation_time = std::chrono::system_clock::now()));
			});

			const auto found = find<Type>(id);
			if (!found.has_value())
			{
				throw std::runtime_error("[database::players::create] Insertion failed");
			}

			return found.value();
		}

		template <database_type_t Type>
		void set_ip_and_port(const std::uint64_t player_id, const std::string& ex_ip, const std::uint16_t ex_port,
			const std::string& in_ip, const std::uint16_t in_port, const std::string& nat_type)
		{
			const auto nat_type_id = get_nat_type_id(nat_type);

			database::access([&](database::database_t& db)
			{
				db.get_database<Type>()->operator()(
					sqlpp::update(player::table)
						.set(player::table.ex_ip = ex_ip,
							 player::table.in_ip = in_ip,
							 player::table.ex_port = ex_port,
							 player::table.in_port = in_port,
							 player::table.nat = nat_type_id)
								.where(player::table.id == player_id));
			});
		}

		template <database_type_t Type>
		std::vector<player> get_player_list(const std::uint64_t user_id)
		{
			return database::access<std::vector<player>>([&](database::database_t& db)
				-> std::vector<player>
			{
				auto results = db.get_database<Type>()->operator()(
					sqlpp::select(
						sqlpp::all_of(player::table), users::user::table.account_id)
							.from(player::table.join(users::user::table).on(users::user::table.id == player::table.user_id))
								.where(player::table.user_id == user_id));

				std::vector<player> list;

				for (auto& row : results)
				{
					list.emplace_back(row);
				}

				return list;
			});
		}

		template <database_type_t Type>
		void delete_player_data(const std::uint64_t player_id)
		{
			return database::access([&](database_t& db)
			{
				db.get_database<Type>()->operator()(
					sqlpp::remove_from(player::table)
						.where(player::table.id == player_id));
			});
		}
	}

	std::optional<player> find(const std::uint64_t id)
	{
		RUN_IMPL(impl::find, id);
	}

	std::optional<player> find_by_index(const std::uint64_t user_id, const std::uint64_t player_index)
	{
		RUN_IMPL(impl::find_by_index, user_id, player_index);
	}

	void set_ip_and_port(const std::uint64_t player_id, const std::string& ex_ip, const std::uint16_t ex_port,
		const std::string& in_ip, const std::uint16_t in_port, const std::string& nat_type)
	{
		RUN_IMPL(impl::set_ip_and_port, player_id, ex_ip, ex_port, in_ip, in_port, nat_type);
	}

	std::vector<player> get_player_list(const std::uint64_t user_id)
	{
		RUN_IMPL(impl::get_player_list, user_id);
	}

	void delete_player_data(const std::uint64_t player_id)
	{
		RUN_IMPL(impl::delete_player_data, player_id);
	}

	class table final : public table_interface
	{
	public:
		void create(database_t& database) override
		{
			database.run_query("mgvdb.players.create");
			database.run_query("mgvdb.players.remove_insert_trigger");
			database.run_query("mgvdb.players.add_insert_trigger");
		}
	};
}

REGISTER_TABLE(database::players::table, 1)
