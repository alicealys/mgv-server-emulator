#include <std_include.hpp>

#include "rankings.hpp"

#include <utils/cryptography.hpp>
#include <utils/string.hpp>

namespace database::rankings
{
	GET_FIELD_C(ranking, std::uint64_t, ranking_id);
	GET_FIELD_C(ranking, std::uint64_t, user_id);
	GET_FIELD_C(ranking, std::uint8_t, ranking_type);
	GET_FIELD_C(ranking, std::uint32_t, ranking_state);
	GET_FIELD_C(ranking, std::uint64_t, rank);
	GET_FIELD_C(ranking, std::uint64_t, rank_number);
	GET_FIELD_C(ranking, std::int32_t, points);
	GET_FIELD_C(ranking, std::uint64_t, account_id);
	GET_FIELD_C(ranking, users::user_loadout_header_t, user_loadout_header);

	namespace impl
	{
		template <database_type_t Type>
		void create_entries(const std::uint64_t user_id)
		{
			database::access([&](database_t& db)
			{
				for (auto i = 0u; i < ranking_type_count; i++)
				{
					const auto found = db.exec<Type>(
						sqlpp::select(ranking::table.ranking_id)
							.from(ranking::table)
								.where(ranking::table.f_user_id == user_id && ranking::table.ranking_type == i));

					if (!found.empty())
					{
						continue;
					}

					db.exec<Type>(
						sqlpp::insert_into(ranking::table)
							.set(ranking::table.f_user_id = user_id,
								 ranking::table.ranking_type = i)
					);
				}
			});
		}
	
		template <database_type_t Type>
		bool set_points(const std::uint64_t user_id, const std::uint8_t ranking_type, const std::int32_t points)
		{
			return database::access<bool>([&](database_t& db)
			{
				const auto result = db.exec<Type>(
					sqlpp::update(ranking::table)
						.set(ranking::table.points = points)
							.where(ranking::table.f_user_id == user_id &&
								   ranking::table.ranking_type == ranking_type)
				);

				return result != 0;
			});
		}

		template <database_type_t Type>
		bool increment_points(const std::uint64_t user_id, const std::uint8_t ranking_type, const std::int32_t count)
		{
			return database::access<bool>([&](database_t& db)
			{
				const auto result = db.exec<Type>(
					sqlpp::update(ranking::table)
						.set(ranking::table.points = ranking::table.points + count)
							.where(ranking::table.f_user_id == user_id && 
								   ranking::table.ranking_type == ranking_type)
				);

				return result != 0;
			});
		}

		template <database_type_t Type>
		bool set_points_if_bigger(const std::uint64_t user_id, const std::uint8_t ranking_type, const std::int32_t points)
		{
			return database::access<bool>([&](database_t& db)
			{
				const auto result = db.exec<Type>(
					sqlpp::update(ranking::table)
						.set(ranking::table.points = points)
							.where(ranking::table.f_user_id == user_id &&
								   ranking::table.ranking_type == ranking_type && ranking::table.points < points)
				);

				return result != 0;
			});
		}

		template <database_type_t Type>
		void reset_points(const std::uint8_t ranking_type)
		{
			return database::access([&](database_t& db)
			{
				db.exec<Type>(
					sqlpp::update(ranking::table)
						.set(ranking::table.points = 0, 
							 ranking::table.player_rank = 0, 
							 ranking::table.player_rank_number = 0)
								.where(ranking::table.ranking_type == ranking_type)
					);
			});
		}

		template <database_type_t Type>
		std::optional<std::uint64_t> get_user_rank(const std::uint64_t user_id, const std::uint8_t ranking_type)
		{
			return database::access<std::optional<std::uint64_t>>([&](database_t& db)
				-> std::optional<std::uint64_t>
			{
				auto results = db.exec<Type>(
					sqlpp::select(ranking::table.player_rank)
						.from(ranking::table)
							.where(ranking::table.ranking_type == ranking_type &&
								   ranking::table.f_user_id == user_id)
					);

				if (results.empty())
				{
					return {};
				}

				return {results.front().player_rank};
			});
		}

		template <database_type_t Type>
		std::optional<ranking> get_user_entry(const std::uint64_t user_id, const std::uint8_t ranking_type)
		{
			return database::access<std::optional<ranking>>([&](database_t& db)
				-> std::optional<ranking>
			{
				const auto joined_tables = ranking::table.join(users::user::table).on(ranking::table.f_user_id == users::user::table.user_id);

				auto results = db.exec<Type>(
					sqlpp::select(sqlpp::all_of(ranking::table), users::user::table.account_id, users::user::table.user_loadout_header)
						.from(joined_tables)
							.where(ranking::table.ranking_type == ranking_type && 
								   ranking::table.f_user_id == user_id)
					);

				if (results.empty())
				{
					return {};
				}

				return ranking(results.front());
			});
		}

		template <database_type_t Type>
		std::vector<ranking> get_account_entries(const std::vector<std::uint64_t>& account_ids, const std::uint8_t ranking_type)
		{
			return database::access<std::vector<ranking>>([&](database_t& db)
				-> std::vector<ranking>
			{
				std::vector<ranking> list;

				const auto joined_tables = ranking::table.join(users::user::table).on(ranking::table.f_user_id == users::user::table.user_id);

				auto results = db.exec<Type>(
					sqlpp::select(sqlpp::all_of(ranking::table), users::user::table.account_id, users::user::table.user_loadout_header)
						.from(joined_tables)
							.where(ranking::table.ranking_type == ranking_type && 
								   users::user::table.account_id.in(sqlpp::value_list(account_ids)))
					);

				for (auto& row : results)
				{
					list.emplace_back(row);
				}

				return list;
			});
		}


		template <database_type_t Type>
		std::vector<ranking> get_entries(const std::uint8_t ranking_type, const std::uint64_t offset, const std::uint32_t num, const std::uint64_t min_rank)
		{
			return database::access<std::vector<ranking>>([&](database_t& db)
				-> std::vector<ranking>
			{
				std::vector<ranking> list;

				const auto joined_tables = ranking::table.join(users::user::table).on(ranking::table.f_user_id == users::user::table.user_id);

				auto results = db.exec<Type>(
					sqlpp::select(sqlpp::all_of(ranking::table), users::user::table.account_id, users::user::table.user_loadout_header)
						.from(joined_tables)
							.where(ranking::table.player_rank >= min_rank && ranking::table.ranking_type == ranking_type)
								.order_by(ranking::table.player_rank_number.asc())
									.limit(num)
										.offset(offset)
					);

				for (auto& row : results)
				{
					list.emplace_back(row);
				}

				return list;
			});
		}
		
		template <database_type_t Type>
		std::vector<ranking> get_entries_with_state(const std::uint8_t ranking_type, const std::uint32_t ranking_state, const std::uint32_t num)
		{
			return database::access<std::vector<ranking>>([&](database_t& db)
				-> std::vector<ranking>
			{
				std::vector<ranking> list;

				const auto joined_tables = ranking::table.join(users::user::table).on(ranking::table.f_user_id == users::user::table.user_id);

				auto results = db.exec<Type>(
					sqlpp::select(sqlpp::all_of(ranking::table), users::user::table.account_id, users::user::table.user_loadout_header)
						.from(joined_tables)
							.where(ranking::table.ranking_state == ranking_state && ranking::table.ranking_type == ranking_type)
								.order_by(ranking::table.player_rank_number.asc())
									.limit(num)
					);

				for (auto& row : results)
				{
					list.emplace_back(row);
				}

				return list;
			});
		}
		

		template <database_type_t Type>
		void set_ranking_state(const std::uint64_t ranking_id, const std::uint32_t state)
		{
			return database::access([&](database_t& db)
			{
				db.exec<Type>(
					sqlpp::update(ranking::table)
						.set(ranking::table.ranking_state = state)
							.where(ranking::table.ranking_id == ranking_id)
					);
			});
		}

		template <database_type_t Type>
		std::size_t get_entry_count(const std::uint8_t ranking_type)
		{
			return database::access<std::size_t>([&](database_t& db) -> std::size_t
			{
				auto results = db.exec<Type>(
					sqlpp::select(sqlpp::count(1))
						.from(ranking::table)
							.where(ranking::table.player_rank != 0 && ranking::table.ranking_type == ranking_type)
					);

				if (results.empty())
				{
					return 0ull;
				}

				return results.front().count.value();
			});
		}

		template <database_type_t Type>
		void delete_player_data(const std::uint64_t user_id)
		{
			database::access([&](database_t& db)
			{
				db.exec<Type>(
					sqlpp::remove_from(ranking::table)
						.where(ranking::table.f_user_id == user_id));
			});
		}
	}

	void create_entries(const std::uint64_t user_id)
	{
		RUN_IMPL(impl::create_entries, user_id);
	}

	bool set_points(const std::uint64_t user_id, const std::uint8_t ranking_type, const std::int32_t value)
	{
		RUN_IMPL(impl::set_points, user_id, ranking_type, value);
	}

	bool increment_points(const std::uint64_t user_id, const std::uint8_t ranking_type, const std::int32_t count)
	{
		RUN_IMPL(impl::increment_points, user_id, ranking_type, count);
	}

	bool set_points_if_bigger(const std::uint64_t user_id, const std::uint8_t ranking_type, const std::int32_t value)
	{
		RUN_IMPL(impl::set_points_if_bigger, user_id, ranking_type, value);
	}

	void reset_points(const std::uint8_t ranking_type)
	{
		RUN_IMPL(impl::reset_points, ranking_type);
	}

	std::optional<std::uint64_t> get_user_rank(const std::uint64_t user_id, const std::uint8_t ranking_type)
	{
		RUN_IMPL(impl::get_user_rank, user_id, ranking_type);
	}

	std::optional<ranking> get_user_entry(const std::uint64_t user_id, const std::uint8_t ranking_type)
	{
		RUN_IMPL(impl::get_user_entry, user_id, ranking_type);
	}

	std::vector<ranking> get_account_entries(const std::vector<std::uint64_t>& account_ids, const std::uint8_t ranking_type)
	{
		RUN_IMPL(impl::get_account_entries, account_ids, ranking_type);
	}

	std::vector<ranking> get_entries(const std::uint8_t ranking_type, const std::uint64_t offset, const std::uint32_t num, const std::uint64_t min_rank)
	{
		RUN_IMPL(impl::get_entries, ranking_type, offset, num, min_rank);
	}

	std::vector<ranking> get_entries_with_state(const std::uint8_t ranking_type, const std::uint32_t ranking_state, const std::uint32_t num)
	{
		RUN_IMPL(impl::get_entries_with_state, ranking_type, ranking_state, num);
	}

	std::size_t get_entry_count(const std::uint8_t ranking_type)
	{
		RUN_IMPL(impl::get_entry_count, ranking_type);
	}

	void set_ranking_state(const std::uint64_t ranking_id, const std::uint32_t ranking_state)
	{
		RUN_IMPL(impl::set_ranking_state, ranking_id, ranking_state);
	}

	void update_rankings(const std::uint8_t ranking_type)
	{
		database::access([&](database_t& db)
		{
			db.run_query("mgssd.rankings.update_entries", ranking_type);
		});
	}

	void delete_user_data(const std::uint64_t user_id)
	{
		RUN_IMPL(impl::delete_player_data, user_id);
	}

	class table final : public table_interface
	{
	public:
		void create(database_t& database) override
		{
			database.run_query("mgssd.rankings.create");
		}

		void run_tasks(database_t& database) override
		{

		}
	};
}

REGISTER_TABLE(database::rankings::table, 3)
