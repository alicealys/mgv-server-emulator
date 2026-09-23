#include <std_include.hpp>

#include "present_box.hpp"

#include "utils/encoding.hpp"
#include "utils/json_utils.hpp"

#include <utils/cryptography.hpp>
#include <utils/string.hpp>

namespace database::present_box
{
	namespace
	{
		std::uint32_t category_caps[game::REWARD_COUNT] =
		{
			99999, // REWARD_RESOURCE = 0,
			300,   // REWARD_PRODUCTION = 1,
			0, // REWARD_RECIPE = 2,
			0, // REWARD_PRESET_RADIO = 3,
			0, // REWARD_GESTURE = 4,
			0, // REWARD_COMMUNICATION_MARKER = 5,
			0, // REWARD_NAMEPLATE = 6,
			0, // REWARD_PRIVILEGE = 7,
			99999, // REWARD_COIN = 8,
			999999, // REWARD_ENERGY = 9,
			0, // REWARD_BATTLE_PACK = 10,
			0, // REWARD_FACE_PAINT = 11,
			0, // REWARD_CASSETTE = 12,
			0, // REWARD_CHARACTER_SLOT = 13,
		};
	}

	GET_FIELD_C(present_box_entry, std::uint64_t, present_id);
	GET_FIELD_C(present_box_entry, std::uint64_t, player_id);
	GET_FIELD_C(present_box_entry, std::uint32_t, flags);
	GET_FIELD_C(present_box_entry, std::chrono::seconds, expire_date);

	const game::reward_t& present_box_entry::get_reward() const
	{
		return this->reward_;
	}

	namespace impl
	{
		template <database_type_t Type>
		std::optional<present_box_entry> find_item(const std::uint64_t present_id)
		{
			return database::access<std::optional<present_box_entry>>([&](database::database_t& db)
				-> std::optional<present_box_entry>
			{
				auto results = db.exec<Type>(
					sqlpp::select(sqlpp::all_of(present_box_entry::table)).from(present_box_entry::table)
						.where(present_box_entry::table.present_id == present_id));

				if (results.empty())
				{
					return {};
				}

				const auto& row = results.front();
				return {present_box_entry(row)};
			});
		}

		template <database_type_t Type>
		bool delete_item(const std::uint64_t present_id)
		{
			return database::access<bool>([&](database::database_t& db)
			{
				auto results = db.exec<Type>(
					sqlpp::remove_from(present_box_entry::table)
						.where(present_box_entry::table.present_id == present_id));
				return results != 0ull;
			});
		}
		
		template <database_type_t Type>
		void remove_new_flag(const std::uint64_t present_id)
		{
			database::access([&](database::database_t& db)
			{
				db.exec<Type>(
					sqlpp::update(present_box_entry::table)
						.set(present_box_entry::table.flags = present_box_entry::table.flags & ~static_cast<std::uint32_t>(present_flag_new))
							.where(present_box_entry::table.present_id == present_id));
			});
		}

		template <database_type_t Type>
		std::vector<present_box_entry> get_all_items(const std::uint64_t player_id)
		{
			return database::access<std::vector<present_box_entry>>([&](database::database_t& db)
				-> std::vector<present_box_entry>
			{
				auto results = db.exec<Type>(
					sqlpp::select(sqlpp::all_of(present_box_entry::table)).from(present_box_entry::table)
						.where(present_box_entry::table.f_player_id == player_id));

				std::vector<present_box_entry> list;

				for (auto& row : results)
				{
					list.emplace_back(row);
				}

				return list;
			});
		}
		
		template <database_type_t Type>
		void delete_all_items(const std::uint64_t player_id)
		{
			database::access([&](database::database_t& db)
			{
				db.exec<Type>(
					sqlpp::remove_from(present_box_entry::table)
						.where(present_box_entry::table.f_player_id == player_id));
			});
		}
		
		template <database_type_t Type>
		void delete_exceeding(const std::uint64_t player_id)
		{
			return database::access([&](database::database_t& db)
			{
				if constexpr (Type == database_mysql)
				{
					const auto keep =
						sqlpp::select(present_box_entry::table.present_id).from(present_box_entry::table)
							.where(present_box_entry::table.f_player_id == player_id)
								.order_by(present_box_entry::table.expire_date.desc()).limit(max_entries).as(sqlpp::alias::x);

					db.exec<Type>(sqlpp::remove_from(present_box_entry::table)
						.using_(present_box_entry::table.left_outer_join(keep)
							.on(present_box_entry::table.present_id == keep.present_id))
						.where(keep.present_id.is_null() && present_box_entry::table.f_player_id == player_id));
				}
				else
				{
					const auto keep =
						sqlpp::select(present_box_entry::table.present_id).from(present_box_entry::table)
							.where(present_box_entry::table.f_player_id == player_id)
								.order_by(present_box_entry::table.expire_date.desc()).limit(max_entries);

					db.exec<Type>(sqlpp::remove_from(present_box_entry::table)
						.where(present_box_entry::table.f_player_id == player_id && present_box_entry::table.present_id.not_in(keep)));
				}
			});
		}

		template <database_type_t Type>
		void add_item_internal(database_t& db, const std::uint64_t player_id, const std::uint32_t flags, 
			const std::chrono::seconds expire_date, game::reward_t& item, const std::size_t depth = 0u)
		{
			if (depth > 3)
			{
				return;
			}

			const auto can_stack = category_caps[item.category] > 0;
			if (can_stack)
			{
				const auto existing = db.exec<Type>(
					sqlpp::select(sqlpp::all_of(present_box_entry::table)).from(present_box_entry::table)
						.where(present_box_entry::table.f_player_id == player_id &&
							present_box_entry::table.item_code == item.code &&
							present_box_entry::table.item_category == item.category &&
							present_box_entry::table.item_param1 == item.param1 &&
							present_box_entry::table.item_param2 == item.param2 &&
							present_box_entry::table.item_param3 == item.param3 &&
							present_box_entry::table.item_param4 == item.param4 &&
							present_box_entry::table.item_param5 == item.param5 &&
							present_box_entry::table.item_num < category_caps[item.category]
						)
				);

				const auto capped_amount = std::min(item.num, category_caps[item.category]);
				if (existing.empty())
				{
					db.exec<Type>(sqlpp::insert_into(present_box_entry::table)
						.set(present_box_entry::table.f_player_id = player_id,
							present_box_entry::table.flags = flags,
							present_box_entry::table.expire_date = std::chrono::system_clock::time_point(expire_date),
							present_box_entry::table.item_num = capped_amount,
							present_box_entry::table.item_category = item.category,
							present_box_entry::table.item_code = item.code,
							present_box_entry::table.item_param1 = item.param1,
							present_box_entry::table.item_param2 = item.param2,
							present_box_entry::table.item_param3 = item.param3,
							present_box_entry::table.item_param4 = item.param4,
							present_box_entry::table.item_param5 = item.param5
						)
					);

					item.num -= capped_amount;
				}
				else
				{
					present_box_entry entry(existing.front());
					const auto add_amount = std::min(static_cast<std::uint32_t>(category_caps[item.category] - entry.get_reward().num), capped_amount);
				
					db.exec<Type>(sqlpp::update(present_box_entry::table)
						.set(present_box_entry::table.item_num = present_box_entry::table.item_num + add_amount,
							 present_box_entry::table.expire_date = std::chrono::system_clock::time_point(expire_date))
								.where(present_box_entry::table.present_id == entry.get_present_id()));
				
					item.num -= add_amount;
				}

				if (item.num > 0)
				{
					add_item_internal<Type>(db, player_id, flags, expire_date, item, depth + 1);
				}
			}
			else
			{
				db.exec<Type>(sqlpp::insert_into(present_box_entry::table)
					.set(present_box_entry::table.f_player_id = player_id,
						present_box_entry::table.flags = flags,
						present_box_entry::table.expire_date = std::chrono::system_clock::time_point(expire_date),
						present_box_entry::table.item_num = item.num,
						present_box_entry::table.item_category = item.category,
						present_box_entry::table.item_code = item.code,
						present_box_entry::table.item_param1 = item.param1,
						present_box_entry::table.item_param2 = item.param2,
						present_box_entry::table.item_param3 = item.param3,
						present_box_entry::table.item_param4 = item.param4,
						present_box_entry::table.item_param5 = item.param5
					)
				);
			}
		}

		template <database_type_t Type>
		void add_item(const std::uint64_t player_id, const std::uint32_t flags, const std::chrono::seconds expire_date, const game::reward_t& item)
		{
			database::access([&](database::database_t& db)
			{
				game::reward_t add_item{item};
				db.get_database<Type>()->start_transaction();
				add_item_internal<Type>(db, player_id, flags, expire_date, add_item);
				db.get_database<Type>()->commit_transaction();
				delete_exceeding<Type>(player_id);
			});
		}

		template <database_type_t Type>
		bool has_new_item(const std::uint64_t player_id)
		{
			return database::access<bool>([&](database::database_t& db)
				-> bool
			{
				auto results = db.exec<Type>(
					sqlpp::select(
						sqlpp::count(1))
							.from(present_box_entry::table)
								.where(present_box_entry::table.f_player_id == player_id && 
									   (present_box_entry::table.flags & static_cast<std::uint32_t>(present_flag_new)) != 0)
				);

				return results.front().count.value() != 0ull;
			});
		}

		template <database_type_t Type>
		std::size_t get_present_count(const std::uint64_t player_id)
		{
			return database::access<std::size_t>([&](database::database_t& db)
			{
				auto results = db.get_database<Type>()->operator()(
					sqlpp::select(
						sqlpp::count(1))
							.from(present_box_entry::table)
								.where(present_box_entry::table.f_player_id == player_id));

				return results.front().count.value();
			});
		}

		template <database_type_t Type>
		void delete_player_data(const std::uint64_t player_id)
		{
			database::access([&](database_t& db)
			{
				db.exec<Type>(
					sqlpp::remove_from(present_box_entry::table)
						.where(present_box_entry::table.f_player_id == player_id));
			});
		}
	}

	void present_box_entry::to_json(json::value& data) const
	{
		data["expire_date"] = this->get_expire_date().count();
		data["id"] = this->get_present_id();
		data["is_expire"] = this->get_flags() & present_flag_expire;
		data["is_new"] = this->get_flags() & present_flag_new;
		data["is_purchase"] = this->get_flags() & present_flag_purchase;
		data["text_id"] = 229691447841577;
		this->to_json_item(data["item"]);
	}

	void present_box_entry::to_json_item(json::value& data) const
	{
		const auto& reward = this->get_reward();
		data["category"] = reward.category;
		data["code"] = reward.code;
		data["num"] = reward.num;
		data["param1"] = reward.param1;
		data["param2"] = reward.param2;
		data["param3"] = reward.param3;
		data["param4"] = reward.param4;
		data["param5"] = reward.param5;
	}

	std::optional<present_box_entry> find_item(const std::uint64_t present_id)
	{
		RUN_IMPL(impl::find_item, present_id);
	}

	bool delete_item(const std::uint64_t present_id)
	{
		RUN_IMPL(impl::delete_item, present_id);
	}

	void remove_new_flag(const std::uint64_t present_id)
	{
		RUN_IMPL(impl::remove_new_flag, present_id);
	}

	std::vector<present_box_entry> get_all_items(const std::uint64_t player_id)
	{
		RUN_IMPL(impl::get_all_items, player_id);
	}

	std::size_t get_present_count(const std::uint64_t player_id)
	{
		RUN_IMPL(impl::get_present_count, player_id);
	}

	void delete_all_items(const std::uint64_t player_id)
	{
		RUN_IMPL(impl::delete_all_items, player_id);
	}

	void add_item(const std::uint64_t player_id, const std::uint32_t flags, const std::chrono::seconds expire_date, const game::reward_t& item)
	{
		RUN_IMPL(impl::add_item, player_id, flags, expire_date, item);
	}

	bool has_new_item(const std::uint64_t player_id)
	{
		RUN_IMPL(impl::has_new_item, player_id);
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
			game::reward_t param{};
			param.code = 1994203852;
			param.category = game::REWARD_PRODUCTION;
			param.num = 301;
			add_item(3, present_flag_expire, std::chrono::seconds(std::time(nullptr)), param);
			database.run_query("mgssd.present_box_entries.create");
		}
	};
}

REGISTER_TABLE(database::present_box::table, 2)
