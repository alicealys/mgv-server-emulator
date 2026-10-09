#include <std_include.hpp>

#include "matching.hpp"
#include "users.hpp"
#include "../auth.hpp"

#include <utils/cryptography.hpp>
#include <utils/string.hpp>

namespace database::matching
{
	GET_FIELD_C(matching_member, std::uint64_t, member_id);
	GET_FIELD_C(matching_member, std::uint64_t, room_id);
	GET_FIELD_C(matching_member, std::uint64_t, player_id);
	GET_FIELD_C(matching_member, std::uint64_t, account_id);
	GET_FIELD_C(matching_member, std::uint64_t, user_id);
	GET_FIELD_C(matching_member, std::uint16_t, ex_port);
	GET_FIELD_C(matching_member, std::uint32_t, migration_sequence);
	GET_FIELD_C(matching_member, std::chrono::seconds, create_date);
	GET_FIELD_C(matching_member, std::string, ex_ip);
	GET_FIELD_C(matching_member, std::string, member_data);

	std::chrono::seconds matching_member::get_past_time() const
	{
		const auto now = std::chrono::system_clock::now();
		const auto diff = now.time_since_epoch() - this->create_date_;
		return std::chrono::duration_cast<std::chrono::seconds>(diff);
	}

	GET_FIELD_C(matching_room, std::uint64_t, room_id);
	GET_FIELD_C(matching_room, std::uint64_t, owner_id);
	GET_FIELD_C(matching_room, std::uint8_t, max_slot);
	GET_FIELD_C(matching_room, std::uint8_t, region_matching_level);
	GET_FIELD_C(matching_room, std::uint8_t, flag_attr);
	GET_FIELD_C(matching_room, std::uint8_t, flag_filter);
	GET_FIELD_C(matching_room, std::uint32_t, mission_id);
	GET_FIELD_C(matching_room, std::uint32_t, status);
	GET_FIELD_C(matching_room, std::string, password);
	GET_FIELD_C(matching_room, std::chrono::seconds, create_date);

	namespace impl
	{
		template <database_type_t Type>
		std::uint64_t create_room(const std::uint64_t owner_id, const create_param_t& param)
		{
			return database::access<std::uint64_t>([&](database_t& db)
			{
				auto result = db.exec<Type>(
					sqlpp::insert_into(matching_room::table)
						.set(matching_room::table.owner_id = owner_id,
							 matching_room::table.max_slot = param.max_slot,
							 matching_room::table.region_matching_level = param.region_matching_level,
							 matching_room::table.flag_attr = param.flag_attr,
							 matching_room::table.flag_filter = 0,
							 matching_room::table.int_attr_01 = param.room_searchable_int_attr_external[0],
							 matching_room::table.int_attr_02 = param.room_searchable_int_attr_external[1],
							 matching_room::table.int_attr_03 = param.room_searchable_int_attr_external[2],
							 matching_room::table.int_attr_04 = param.room_searchable_int_attr_external[3],
							 matching_room::table.int_attr_05 = param.room_searchable_int_attr_external[4],
							 matching_room::table.int_attr_06 = param.room_searchable_int_attr_external[5],
							 matching_room::table.int_attr_07 = param.room_searchable_int_attr_external[6],
							 matching_room::table.int_attr_08 = param.room_searchable_int_attr_external[7],
							 matching_room::table.int_attr_09 = param.room_searchable_int_attr_external[8],
							 matching_room::table.int_attr_10 = param.room_searchable_int_attr_external[9],
							 matching_room::table.int_attr_11 = param.room_searchable_int_attr_external[10],
							 matching_room::table.int_attr_12 = param.room_searchable_int_attr_external[11],
							 matching_room::table.int_attr_13 = param.room_searchable_int_attr_external[12],
							 matching_room::table.int_attr_14 = param.room_searchable_int_attr_external[13],
							 matching_room::table.int_attr_15 = param.room_searchable_int_attr_external[14],
							 matching_room::table.int_attr_16 = param.room_searchable_int_attr_external[15],
							 matching_room::table.bin_attr_01 = param.room_searchable_bin_attr_external[0].data,
							 matching_room::table.bin_attr_02 = param.room_searchable_bin_attr_external[1].data,
							 matching_room::table.create_date = std::chrono::system_clock::now()
						));
				return result;
			});
		}

		template <database_type_t Type>
		std::optional<matching_room> get_room(const std::uint64_t room_id)
		{
			return database::access<std::optional<matching_room>>([&](database_t& db)
				-> std::optional<matching_room>
			{
				auto results = db.exec<Type>(
					sqlpp::select(sqlpp::all_of(matching_room::table))
						.from(matching_room::table)
							.where(matching_room::table.room_id == room_id)
				);

				std::optional<matching_room> list;

				if (!results.empty())
				{
					list.emplace(results.front());
				}

				return list;
			});
		}

		template <database_type_t Type>
		std::optional<matching_room> get_room_from_member(const std::uint64_t player_id)
		{
			return database::access<std::optional<matching_room>>([&](database_t& db)
				-> std::optional<matching_room>
			{
				const auto joined_tables = matching_room::table.join(matching_member::table)
					.on(matching_room::table.room_id == matching_member::table.f_room_id);

				const auto matching_member = 
					sqlpp::select(sqlpp::all_of(matching_room::table))
						.from(joined_tables)
							.where(matching_member::table.f_player_id == player_id).limit(1u);

				auto results = db.exec<Type>(matching_member);

				std::optional<matching_room> list;

				if (!results.empty())
				{
					list.emplace(results.front());
				}

				return list;
			});
		}

		template <database_type_t Type>
		std::vector<matching_room> search_rooms(const std::uint64_t player_id, const search_param_t& param)
		{
			return database::access<std::vector<matching_room>>([&](database_t& db)
				-> std::vector<matching_room>
			{
				auto not_full = 
					sqlpp::select(sqlpp::count(1))
						.from(matching_member::table)
							.where(matching_member::table.f_room_id == matching_room::table.room_id) 
								+ matching_room::table.reserve_num < matching_room::table.max_slot;
				
				auto not_member_of = 
					sqlpp::select(sqlpp::count(1))
						.from(matching_member::table)
							.where(matching_member::table.f_player_id == player_id && 
								   matching_member::table.f_room_id == matching_room::table.room_id) == 0;

				auto param_match = 
					//matching_room::table.int_attr_01 == param.int_attr_param[0] &&
					//matching_room::table.int_attr_02 == param.int_attr_param[1] &&
					//matching_room::table.int_attr_03 == param.int_attr_param[2] &&
					//matching_room::table.int_attr_04 == param.int_attr_param[3] &&
					//matching_room::table.int_attr_05 == param.int_attr_param[4] &&
					//matching_room::table.int_attr_06 == param.int_attr_param[5] &&
					//matching_room::table.int_attr_07 == param.int_attr_param[6] &&
					matching_room::table.int_attr_08 == param.int_attr_param[7] &&
					matching_room::table.int_attr_09 == param.int_attr_param[8]// &&
					//matching_room::table.int_attr_10 == param.int_attr_param[9] &&
					//matching_room::table.int_attr_11 == param.int_attr_param[10] &&
					//matching_room::table.int_attr_12 == param.int_attr_param[11] &&
					//matching_room::table.int_attr_13 == param.int_attr_param[12] &&
					//matching_room::table.int_attr_14 == param.int_attr_param[13] &&
					//matching_room::table.int_attr_15 == param.int_attr_param[14] &&
					//matching_room::table.int_attr_16 == param.int_attr_param[15]
				;

				auto status_match = matching_room::table.status == static_cast<std::uint32_t>(status_init);

				auto region_match = matching_room::table.region_matching_level <= param.region_matching_level;

				auto search_rooms = 
					sqlpp::select(sqlpp::all_of(matching_room::table))
						.from(matching_room::table)
							.where(matching_room::table.owner_id != player_id && 
								   not_member_of && not_full && region_match && param_match)
								.limit(param.max);

				auto results = db.exec<Type>(search_rooms);

				std::vector<matching_room> list;

				for (auto& row : results)
				{
					list.emplace_back(row);
				}

				return list;
			});
		}

		template <database_type_t Type>
		std::vector<matching_member> get_members(const std::uint64_t room_id)
		{
			return database::access<std::vector<matching_member>>([&](database_t& db)
				-> std::vector<matching_member>
			{
				const auto joined_tables = matching_member::table.join(users::user::table)
					.on(matching_member::table.f_player_id == users::user::table.current_player_id);

				auto results = db.exec<Type>(
					sqlpp::select(sqlpp::all_of(matching_member::table),
								  users::user::table.account_id,
								  users::user::table.user_id,
								  users::user::table.ex_ip,
								  users::user::table.ex_port)
						.from(joined_tables)
							.where(matching_member::table.f_room_id == room_id)
				);

				std::vector<matching_member> list;

				for (auto& row : results)
				{
					list.emplace_back(row);
				}

				return list;
			});
		}

		template <database_type_t Type>
		void remove_member1(const std::uint64_t room_id, const std::uint64_t player_id)
		{
			return database::access([&](database_t& db)
			{
				db.exec<Type>(
					sqlpp::remove_from(matching_member::table)
						.where(matching_member::table.f_room_id == room_id && matching_member::table.f_player_id == player_id)
				);
			});
		}
		
		template <database_type_t Type>
		void remove_member2(const std::uint64_t player_id)
		{
			return database::access([&](database_t& db)
			{
				db.exec<Type>(
					sqlpp::remove_from(matching_member::table)
						.where(matching_member::table.f_player_id == player_id)
				);
			});
		}

		template <database_type_t Type>
		std::uint64_t add_member(const std::uint64_t room_id, const std::uint64_t player_id, const std::string& data)
		{
			return database::access<std::uint64_t>([&](database_t& db)
			{
				return db.exec<Type>(
					sqlpp::insert_into(matching_member::table)
						.set(matching_member::table.f_room_id = room_id, 
							 matching_member::table.member_data = data,
							 matching_member::table.create_date = std::chrono::system_clock::now(),
							 matching_member::table.f_player_id = player_id)
				);
			});
		}

		template <database_type_t Type>
		void set_member_data(const std::uint64_t player_id, const std::string& data)
		{
			database::access([&](database_t& db)
			{
				db.exec<Type>(
					sqlpp::update(matching_member::table)
						.set(matching_member::table.member_data = data)
							.where(matching_member::table.f_player_id == player_id)
				);
			});
		}

		template <database_type_t Type>
		void set_member_migration_sequence(const std::uint64_t player_id, const std::uint32_t migration_sequence)
		{
			database::access([&](database_t& db)
			{
				db.exec<Type>(
					sqlpp::update(matching_member::table)
						.set(matching_member::table.migration_sequence = migration_sequence)
							.where(matching_member::table.f_player_id == player_id)
				);
			});
		}

		template <database_type_t Type>
		void update_room1(const std::uint64_t room_id, const set_data_external_param_t& param)
		{
			return database::access([&](database_t& db)
			{
				db.exec<Type>(
					sqlpp::update(matching_room::table)
						.set(matching_room::table.region_matching_level = param.region_matching_level,
							 matching_room::table.int_attr_01 = param.room_searchable_int_attr_external[0],
							 matching_room::table.int_attr_02 = param.room_searchable_int_attr_external[1],
							 matching_room::table.int_attr_03 = param.room_searchable_int_attr_external[2],
							 matching_room::table.int_attr_04 = param.room_searchable_int_attr_external[3],
							 matching_room::table.int_attr_05 = param.room_searchable_int_attr_external[4],
							 matching_room::table.int_attr_06 = param.room_searchable_int_attr_external[5],
							 matching_room::table.int_attr_07 = param.room_searchable_int_attr_external[6],
							 matching_room::table.int_attr_08 = param.room_searchable_int_attr_external[7],
							 matching_room::table.int_attr_09 = param.room_searchable_int_attr_external[8],
							 matching_room::table.int_attr_10 = param.room_searchable_int_attr_external[9],
							 matching_room::table.int_attr_11 = param.room_searchable_int_attr_external[10],
							 matching_room::table.int_attr_12 = param.room_searchable_int_attr_external[11],
							 matching_room::table.int_attr_13 = param.room_searchable_int_attr_external[12],
							 matching_room::table.int_attr_14 = param.room_searchable_int_attr_external[13],
							 matching_room::table.int_attr_15 = param.room_searchable_int_attr_external[14],
							 matching_room::table.int_attr_16 = param.room_searchable_int_attr_external[15],
							 matching_room::table.bin_attr_01 = param.room_searchable_bin_attr_external[0].data,
							 matching_room::table.bin_attr_02 = param.room_searchable_bin_attr_external[1].data)
								.where(matching_room::table.room_id == room_id)
				);
			});
		}

		template <database_type_t Type>
		void update_room2(const std::uint64_t room_id, const set_data_internal_param_t& param)
		{
			return database::access([&](database_t& db)
			{
				db.exec<Type>(
					sqlpp::update(matching_room::table)
						.set(matching_room::table.flag_attr = param.flag_attr,
							 matching_room::table.flag_filter = param.flag_filter)
								.where(matching_room::table.room_id == room_id)
				);
			});
		}

		template <database_type_t Type>
		void close_room(const std::uint64_t room_id)
		{
			return database::access([&](database_t& db)
			{
				db.exec<Type>(
					sqlpp::remove_from(matching_member::table)
						.where(matching_member::table.f_room_id == room_id)
				);

				db.exec<Type>(
					sqlpp::remove_from(matching_room::table)
						.where(matching_room::table.room_id == room_id)
				);
			});
		}
		
		template <database_type_t Type>
		void check_close_room(const std::uint64_t room_id)
		{
			return database::access([&](database_t& db)
			{
				const auto no_members = sqlpp::select(sqlpp::count(1))
					.from(matching_member::table)
						.where(matching_member::table.f_room_id == room_id) == 0;

				db.exec<Type>(
					sqlpp::remove_from(matching_room::table)
						.where(matching_room::table.room_id == room_id && no_members)
				);
			});
		}

		template <database_type_t Type>
		void close_room_from_owner(const std::uint64_t owner_id)
		{
			return database::access([&](database_t& db)
			{
				if constexpr (Type == database_mysql)
				{
					const auto target_room =
						sqlpp::select(matching_room::table.room_id)
							.from(matching_room::table)
								.where(matching_room::table.owner_id == owner_id);

					const auto target_room_x = target_room.as(sqlpp::alias::x);

					db.exec<Type>(
						sqlpp::remove_from(matching_member::table)
							.where(matching_member::table.f_room_id.in(target_room))
					);

					db.exec<Type>(
						sqlpp::remove_from(matching_room::table)
							.using_(matching_room::table.left_outer_join(target_room_x)
								.on(matching_room::table.room_id == target_room_x.room_id))
									.where(!target_room_x.room_id.is_null())
					);
				}
				else
				{
					const auto target_room =
						sqlpp::select(matching_room::table.room_id)
							.from(matching_room::table)
								.where(matching_room::table.owner_id == owner_id);

					db.exec<Type>(
						sqlpp::remove_from(matching_member::table)
							.where(matching_member::table.f_room_id.in(target_room))
					);

					db.exec<Type>(
						sqlpp::remove_from(matching_room::table)
							.where(matching_room::table.room_id.in(target_room))
					);
				}
			});
		}

		template <database_type_t Type>
		bool migrate_room_owner(const std::uint64_t room_id, const std::uint64_t owner_id)
		{
			return database::access<bool>([&](database_t& db)
			{
				const auto new_owner =
					sqlpp::select(matching_member::table.f_player_id)
						.from(matching_member::table)
							.where(matching_member::table.f_room_id == room_id && 
								   matching_member::table.f_player_id != owner_id)
									.limit(1u);
				
				const auto result = db.exec<Type>(
					sqlpp::update(matching_room::table)
						.set(matching_room::table.owner_id = new_owner)
							.where(matching_room::table.room_id == room_id && 
								   matching_room::table.owner_id == owner_id && new_owner.is_not_null())
				);

				return result != 0ull;
			});
		}

		template <database_type_t Type>
		void set_room_owner(const std::uint64_t room_id, const std::uint64_t owner_id)
		{
			database::access([&](database_t& db)
			{
				db.exec<Type>(
					sqlpp::update(matching_room::table)
						.set(matching_room::table.owner_id = owner_id)
							.where(matching_room::table.room_id == room_id)
				);
			});
		}

		template <database_type_t Type>
		void set_room_mission_id(const std::uint64_t room_id, const std::uint64_t mission_id)
		{
			database::access([&](database_t& db)
			{
				db.exec<Type>(
					sqlpp::update(matching_room::table)
						.set(matching_room::table.mission_id = mission_id)
							.where(matching_room::table.room_id == room_id)
				);
			});
		}

		template <database_type_t Type>
		void set_room_status(const std::uint64_t room_id, const std::uint64_t status)
		{
			database::access([&](database_t& db)
			{
				db.exec<Type>(
					sqlpp::update(matching_room::table)
						.set(matching_room::table.status = status)
							.where(matching_room::table.room_id == room_id)
				);
			});
		}
		
		template <database_type_t Type>
		void set_room_reserve_num(const std::uint64_t room_id, const std::uint8_t reserve_num)
		{
			database::access([&](database_t& db)
			{
				db.exec<Type>(
					sqlpp::update(matching_room::table)
						.set(matching_room::table.reserve_num = reserve_num)
							.where(matching_room::table.room_id == room_id)
				);
			});
		}

		template <database_type_t Type>
		std::size_t get_total_member_count()
		{
			return database::access<std::size_t>([&](database_t& db)
				-> std::size_t
			{
				const auto result = db.exec<Type>(
					sqlpp::select(sqlpp::count(1))
						.from(matching_member::table)
							.unconditionally());

				if (result.empty())
				{
					return 0ull;
				}
				
				return result.front().count.value();
			});
		}

		template <database_type_t Type>
		std::size_t get_total_room_count()
		{
			return database::access<std::size_t>([&](database_t& db)
				-> std::size_t
			{
				const auto result = db.exec<Type>(
					sqlpp::select(sqlpp::count(1))
						.from(matching_room::table)
							.unconditionally());

				if (result.empty())
				{
					return 0ull;
				}
				
				return result.front().count.value();
			});
		}

		template <database_type_t Type>
		void delete_empty_rooms(database_t& db)
		{
			if constexpr (Type == database_mysql)
			{
				const auto active_rooms =
					sqlpp::select(matching_member::table.f_room_id)
						.from(matching_member::table)
							.unconditionally()
								.group_by(matching_member::table.f_room_id)
									.as(sqlpp::alias::x);

				db.exec<Type>(
					sqlpp::remove_from(matching_room::table)
						.using_(matching_room::table.left_outer_join(active_rooms)
							.on(matching_room::table.room_id == active_rooms.f_room_id))
								.where(active_rooms.f_room_id.is_null() && 
									   matching_room::table.create_date < std::chrono::system_clock::now() - 10s)
				);
			}
			else
			{
				const auto active_rooms =
					sqlpp::select(matching_member::table.f_room_id)
						.from(matching_member::table)
							.unconditionally()
								.group_by(matching_member::table.f_room_id);

				db.exec<Type>(sqlpp::remove_from(matching_room::table)
					.where(matching_room::table.room_id.not_in(active_rooms)));
			}
		}

		template <database_type_t Type>
		void delete_inactive_members(database_t& db)
		{
			if constexpr (Type == database_mysql)
			{
				const auto active_members =
					sqlpp::select(users::user::table.current_player_id)
						.from(users::user::table)
							.where(users::user::table.last_update >= std::chrono::system_clock::now() - database::vars.session_timeout)
								.as(sqlpp::alias::x);

				db.exec<Type>(
					sqlpp::remove_from(matching_member::table)
						.using_(matching_member::table.left_outer_join(active_members)
							.on(matching_member::table.f_player_id == active_members.current_player_id))
								.where(active_members.current_player_id.is_null())
				);
			}
			else
			{
				const auto active_members =
					sqlpp::select(users::user::table.current_player_id)
						.from(users::user::table)
							.where(users::user::table.last_update >= std::chrono::system_clock::now() - database::vars.session_timeout);

				db.exec<Type>(sqlpp::remove_from(matching_member::table)
					.where(matching_member::table.f_player_id.not_in(active_members)));
			}
		}

		template <database_type_t Type>
		void migrate_room_owners(database_t& db)
		{
			auto select_owner = sqlpp::select(matching_member::table.f_player_id)
				.from(matching_member::table)
					.where(matching_member::table.f_room_id == matching_room::table.room_id &&
						   matching_member::table.f_player_id == matching_room::table.owner_id);

			auto bad_rooms = sqlpp::select(matching_room::table.room_id, matching_room::table.owner_id)
				.from(matching_room::table)
					.where(select_owner.is_null()).limit(32u);

			auto result = db.exec<Type>(bad_rooms);
			for (auto& row : result)
			{
				migrate_room_owner<Type>(row.room_id, row.owner_id);
			}
		}
	}

	std::uint64_t create_room(const std::uint64_t owner_id, const create_param_t& param)
	{
		RUN_IMPL(impl::create_room, owner_id, param);
	}

	std::optional<matching_room> get_room(const std::uint64_t room_id)
	{
		RUN_IMPL(impl::get_room, room_id);
	}

	std::optional<matching_room> get_room_from_member(const std::uint64_t player_id)
	{
		RUN_IMPL(impl::get_room_from_member, player_id);
	}

	std::vector<matching_room> search_rooms(const std::uint64_t player_id, const search_param_t& param)
	{
		RUN_IMPL(impl::search_rooms, player_id, param);
	}

	std::vector<matching_member> get_members(const std::uint64_t room_id)
	{
		RUN_IMPL(impl::get_members, room_id);
	}

	void delete_empty_rooms(database_t& db)
	{
		RUN_IMPL(impl::delete_empty_rooms, db);
	}

	void delete_inactive_members(database_t& db)
	{
		RUN_IMPL(impl::delete_inactive_members, db);
	}

	void migrate_room_owners(database_t& db)
	{
		RUN_IMPL(impl::migrate_room_owners, db);
	}

	void remove_member(const std::uint64_t room_id, const std::uint64_t player_id)
	{
		RUN_IMPL(impl::remove_member1, room_id, player_id);
	}

	void remove_member(const std::uint64_t player_id)
	{
		RUN_IMPL(impl::remove_member2, player_id);
	}

	std::uint64_t add_member(const std::uint64_t room_id, const std::uint64_t player_id, const std::string& data)
	{
		RUN_IMPL(impl::add_member, room_id, player_id, data);
	}

	void set_member_data(const std::uint64_t player_id, const std::string& data)
	{
		RUN_IMPL(impl::set_member_data, player_id, data);
	}

	void set_member_migration_sequence(const std::uint64_t player_id, const std::uint32_t migration_sequence)
	{
		RUN_IMPL(impl::set_member_migration_sequence, player_id, migration_sequence);
	}

	void update_room(const std::uint64_t room_id, const set_data_external_param_t& param)
	{
		RUN_IMPL(impl::update_room1, room_id, param);
	}

	void update_room(const std::uint64_t room_id, const set_data_internal_param_t& param)
	{
		RUN_IMPL(impl::update_room2, room_id, param);
	}

	void check_close_room(const std::uint64_t room_id)
	{
		RUN_IMPL(impl::check_close_room, room_id);
	}

	void close_room(const std::uint64_t room_id)
	{
		RUN_IMPL(impl::close_room, room_id);
	}

	void close_room_from_owner(const std::uint64_t owner_id)
	{
		RUN_IMPL(impl::close_room_from_owner, owner_id);
	}

	bool migrate_room_owner(const std::uint64_t room_id, const std::uint64_t owner_id)
	{
		RUN_IMPL(impl::migrate_room_owner, room_id, owner_id);
	}

	void set_room_owner(const std::uint64_t room_id, const std::uint64_t owner_id)
	{
		RUN_IMPL(impl::set_room_owner, room_id, owner_id);
	}

	void set_room_mission_id(const std::uint64_t room_id, const std::uint32_t mission_id)
	{
		RUN_IMPL(impl::set_room_mission_id, room_id, mission_id);
	}

	void set_room_status(const std::uint64_t room_id, const std::uint32_t status)
	{
		RUN_IMPL(impl::set_room_status, room_id, status);
	}

	void set_room_reserve_num(const std::uint64_t room_id, const std::uint8_t num)
	{
		RUN_IMPL(impl::set_room_reserve_num, room_id, num);
	}

	std::size_t get_total_room_count()
	{
		RUN_IMPL(impl::get_total_room_count);
	}
	
	std::size_t get_total_member_count()
	{
		RUN_IMPL(impl::get_total_member_count);
	}

	class table final : public table_interface
	{
	public:
		void create(database_t& database) override
		{
			database.run_query("mgssd.matching_rooms.create");
			database.run_query("mgssd.matching_members.create");
		}

		void run_tasks(database_t& database) override
		{
			static std::chrono::system_clock::time_point last_update;
			const auto now = std::chrono::system_clock::now();

			if (now - last_update < 500ms)
			{
				return;
			}

			last_update = now;

			delete_inactive_members(database);
			delete_empty_rooms(database);
			migrate_room_owners(database);
		}
	};
}

REGISTER_TABLE(database::matching::table, -1)
