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
		namespace exp
		{
			const auto select = sqlpp::select(
				sqlpp::all_of(player::table), users::user::table.account_id)
					.from(player::table.join(users::user::table).on(users::user::table.user_id == player::table.f_user_id));
		}
	}

	GET_FIELD_C(player, std::uint64_t, player_id);
	GET_FIELD_C(player, std::uint64_t, user_id);
	GET_FIELD_C(player, std::uint64_t, account_id);
	GET_FIELD_C(player, std::uint64_t, index);
	GET_FIELD_C(player, std::chrono::microseconds, last_update);
	GET_FIELD_C(player, std::chrono::microseconds, creation_date);

	std::string player::get_name() const
	{
		return std::format("{}_player{:02}", this->get_account_id(), this->get_index() + 1);
	}

	std::uint64_t player::get_id() const
	{
		return this->player_id_;
	}

	namespace impl
	{
		template <database_type_t Type>
		std::optional<player> find(const std::uint64_t player_id)
		{
			return database::access<std::optional<player>>([&](database::database_t& db)
				-> std::optional<player>
			{
				auto results = db.exec<Type>(exp::select.where(player::table.player_id == player_id));

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
				auto results = db.exec<Type>(exp::select
					.where(player::table.f_user_id == user_id && player::table.player_index == player_index));

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
				return db.exec<Type>(
					sqlpp::insert_into(player::table)
						.set(player::table.f_user_id = user_id,
							 player::table.player_creation_date = std::chrono::system_clock::now()));
			});

			const auto found = find<Type>(id);
			if (!found.has_value())
			{
				throw std::runtime_error("[database::players::create] Insertion failed");
			}

			return found.value();
		}

		template <database_type_t Type>
		std::vector<player> get_player_list(const std::uint64_t user_id)
		{
			return database::access<std::vector<player>>([&](database::database_t& db)
				-> std::vector<player>
			{
				auto results = db.exec<Type>(exp::select.where(player::table.f_user_id == user_id));

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
				db.exec<Type>(
					sqlpp::remove_from(player::table)
						.where(player::table.player_id == player_id));
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
			database.run_query("mgssd.players.create");
			database.run_query("mgssd.players.remove_insert_trigger");
			database.run_query("mgssd.players.add_insert_trigger");
		}
	};
}

REGISTER_TABLE(database::players::table, 1)
