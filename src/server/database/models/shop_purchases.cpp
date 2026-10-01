#include <std_include.hpp>

#include "shop_purchases.hpp"
#include "users.hpp"

#include <utils/string.hpp>

namespace database::shop_purchases
{
	namespace
	{
		std::vector<shop_product_t> load_shop_product_list()
		{
			std::vector<shop_product_t> list;

			auto list_j = utils::resources::load_json(RESOURCE_SHOP_PRODUCT_LIST);
			for (auto i = 0ull; i < list_j.size(); i++)
			{
				shop_product_t entry{};
				if (!json::read(entry, list_j[i]))
				{
					console::warning("[shop purchases] invalid shop product at index %lli\n", i);
					continue;
				}

				list.emplace_back(entry);
			}

			return list;
		}

		std::unordered_map<std::uint32_t, shop_product_t> load_shop_product_map()
		{
			std::unordered_map<std::uint32_t, shop_product_t> map;

			const auto list = load_shop_product_list();
			for (const auto& entry : list)
			{
				if (map.contains(entry.product_id))
				{
					console::warning("[shop purchases] duplicate shop product id %i\n", entry.product_id);
					continue;
				}

				map.insert(std::make_pair(entry.product_id, entry));
			}

			return map;
		}

		std::vector<shop_product_t> load_shop_item_list()
		{
			const auto product_map = load_shop_product_map();
			std::vector<shop_product_t> list;

			auto list_j = utils::resources::load_json(RESOURCE_SHOP_ITEM_LIST);
			for (auto i = 0ull; i < list_j.size(); i++)
			{
				std::uint32_t entry{};
				if (!json::read(entry, list_j[i]))
				{
					console::warning("[shop purchases] invalid shop item at index %lli\n", i);
					continue;
				}

				const auto iter = product_map.find(entry);
				if (iter == product_map.end())
				{
					console::warning("[shop purchases] unknown shop item type %i\n", entry);
					continue;
				}

				list.emplace_back(iter->second);
			}

			return list;
		}

		std::unordered_map<std::uint32_t, shop_product_t> load_shop_item_map()
		{
			std::unordered_map<std::uint32_t, shop_product_t> map;

			const auto list = load_shop_item_list();
			for (const auto& entry : list)
			{
				if (map.contains(entry.product_id))
				{
					console::warning("[shop purchases] duplicate shop product id %i\n", entry.product_id);
					continue;
				}

				map.insert(std::make_pair(entry.product_id, entry));
			}

			return map;
		}

	}

	const std::vector<shop_product_t>& get_shop_product_list()
	{
		static const auto list = load_shop_product_list();
		return list;
	}

	const std::unordered_map<std::uint32_t, shop_product_t>& get_shop_product_map()
	{
		static const auto map = load_shop_product_map();
		return map;
	}

	const std::vector<shop_product_t>& get_shop_item_list()
	{
		static const auto list = load_shop_item_list();
		return list;
	}

	const std::unordered_map<std::uint32_t, shop_product_t>& get_shop_item_map()
	{
		static const auto map = load_shop_item_map();
		return map;
	}

	std::optional<shop_product_t> find_product(const std::uint32_t product_type)
	{
		std::optional<shop_product_t> result;

		const auto iter = get_shop_product_map().find(product_type);
		if (iter == get_shop_product_map().end())
		{
			return result;
		}

		result.emplace(iter->second);

		return result;
	}

	std::optional<shop_product_t> find_item(const std::uint32_t product_type)
	{
		std::optional<shop_product_t> result;

		const auto iter = get_shop_item_map().find(product_type);
		if (iter == get_shop_item_map().end())
		{
			return result;
		}

		result.emplace(iter->second);

		return result;
	}

	void shop_product_t::to_json(json::value& data) const
	{
		this->item.to_json(data["item"]);
		data["price"] = this->price;
		data["product_id"] = this->product_id;
	}

	GET_FIELD_C(shop_purchase, std::uint64_t, purchase_id);
	GET_FIELD_C(shop_purchase, std::uint64_t, user_id);
	GET_FIELD_C(shop_purchase, std::uint64_t, lang_id);
	GET_FIELD_C(shop_purchase, std::uint32_t, event_type);
	GET_FIELD_C(shop_purchase, std::uint32_t, product_type);
	GET_FIELD_C(shop_purchase, std::uint32_t, purchase_quantity);
	GET_FIELD_C(shop_purchase, std::uint32_t, coin_quantity);
	GET_FIELD_C(shop_purchase, std::uint32_t, remaining_coin);
	GET_FIELD_C(shop_purchase, std::chrono::seconds, purchase_date);
	GET_FIELD_C(shop_purchase, std::chrono::seconds, expire_date);

	const game::item_t& shop_purchase::get_item() const
	{
		return this->item_;
	}

	void shop_purchase::to_json(json::value& data) const
	{
		data["coin_quantity"] = this->coin_quantity_;
		data["date"] = this->purchase_date_.count();
		this->item_.to_json(data["item"]);
		data["event_type"] = this->event_type_;
		data["expire_date"] = this->expire_date_.count();
		data["item_quantity"] = this->purchase_quantity_;
		data["item_type"] = this->product_type_;
		data["lang_id"] = this->lang_id_;
		data["remaining_coin"] = this->remaining_coin_;
	}

	namespace impl
	{
		template <database_type_t Type>
		std::uint64_t add_entry(const std::uint64_t user_id, const entry_params_t& params)
		{
			const auto date = std::chrono::system_clock::now();
			const auto expire_date = date + purchase_duration;
			const auto remaining_coin = users::get_sv_coins(user_id);

			return database::access<std::uint64_t>([&](database_t& db)
			{
				const auto result = db.get_database<Type>()->operator()(
					sqlpp::insert_into(shop_purchase::table)
						.set(shop_purchase::table.f_user_id = user_id,
							 shop_purchase::table.lang_id = params.lang_id,
							 shop_purchase::table.event_type = params.event_type,
							 shop_purchase::table.product_type = params.product_type,
							 shop_purchase::table.coin_quantity = params.coin_quantity,
							 shop_purchase::table.purchase_quantity = params.purchase_quantity, 
							 shop_purchase::table.remaining_coin = remaining_coin,
							 shop_purchase::table.item_category = params.item.category,
							 shop_purchase::table.item_code = params.item.code,
							 shop_purchase::table.item_param1 = params.item.param1,
							 shop_purchase::table.item_param2 = params.item.param2,
							 shop_purchase::table.item_param3 = params.item.param3,
							 shop_purchase::table.item_param4 = params.item.param4,
							 shop_purchase::table.item_param5 = params.item.param5,
							 shop_purchase::table.item_num = params.item.num,
							 shop_purchase::table.purchase_date = date,
							 shop_purchase::table.expire_date = expire_date
						));
				return result;
			});
		}

		template <database_type_t Type>
		std::unordered_map<std::uint32_t, std::size_t> get_purchase_counts(const std::uint64_t user_id)
		{
			return database::access<std::unordered_map<std::uint32_t, std::size_t>>([&](database_t& db)
				-> std::unordered_map<std::uint32_t, std::size_t>
			{
				std::unordered_map<std::uint32_t, std::size_t> map;

				auto results = db.get_database<Type>()->operator()(
					sqlpp::select(shop_purchase::table.product_type, sqlpp::count(1))
							.from(shop_purchase::table)
								.where(shop_purchase::table.f_user_id == user_id)
									.group_by(shop_purchase::table.product_type));

				for (const auto& row : results)
				{
					map[static_cast<std::uint32_t>(row.product_type)] = row.count;
				}

				return map;
			});
		}
		
		template <database_type_t Type>
		std::size_t get_purchase_count(const std::uint64_t user_id, const std::uint32_t product_type)
		{
			return database::access<std::size_t>([&](database_t& db)
				-> std::size_t
			{
				auto results = db.get_database<Type>()->operator()(
					sqlpp::select(sqlpp::count(1))
							.from(shop_purchase::table)
								.where(shop_purchase::table.f_user_id == user_id && shop_purchase::table.product_type == product_type));

				if (results.empty())
				{
					return 0ull;
				}

				return static_cast<std::size_t>(results.front().count);
			});
		}

		template <database_type_t Type>
		std::vector<shop_purchase> get_history(const std::uint64_t user_id, const std::uint32_t offset, const std::uint32_t limit)
		{
			return database::access<std::vector<shop_purchase>>([&](database_t& db)
				-> std::vector<shop_purchase>
			{
				const auto now = std::chrono::system_clock::now();
				auto results = db.get_database<Type>()->operator()(
					sqlpp::select(sqlpp::all_of(shop_purchase::table))
							.from(shop_purchase::table)
								.where(shop_purchase::table.f_user_id == user_id && shop_purchase::table.expire_date > now)
									.order_by(shop_purchase::table.purchase_date.desc()).limit(limit).offset(offset));

				std::vector<shop_purchase> list;

				for (const auto& row : results)
				{
					list.emplace_back(row);
				}

				return list;
			});
		}

		template <database_type_t Type>
		std::size_t get_history_size(const std::uint64_t user_id)
		{
			return database::access<std::size_t>([&](database_t& db)
				-> std::size_t
			{
				const auto now = std::chrono::system_clock::now();
				auto results = db.get_database<Type>()->operator()(
					sqlpp::select(sqlpp::count(1))
							.from(shop_purchase::table)
								.where(shop_purchase::table.f_user_id == user_id && shop_purchase::table.expire_date > now));
				
				if (results.empty())
				{
					return 0u;
				}

				return static_cast<std::size_t>(results.front().count);
			});
		}

		template <database_type_t Type>
		std::optional<shop_purchase> get_last_item_purchase(const std::uint64_t user_id, const std::uint32_t item_type)
		{
			return database::access<std::optional<shop_purchase>>([&](database_t& db)
				-> std::optional<shop_purchase>
			{
				auto results = db.get_database<Type>()->operator()(
					sqlpp::select(sqlpp::all_of(shop_purchase::table))
							.from(shop_purchase::table)
								.where(shop_purchase::table.f_user_id == user_id && shop_purchase::table.product_type == item_type)
									.order_by(shop_purchase::table.purchase_date.desc()).limit(1u));

				if (results.empty())
				{
					return {};
				}

				return shop_purchase(results.front());
			});
		}

		template <database_type_t Type>
		std::optional<shop_purchase> get_last_item_purchase_range(const std::uint64_t user_id, const std::uint32_t item_type_beg, const std::uint32_t item_type_end)
		{
			return database::access<std::optional<shop_purchase>>([&](database_t& db)
				-> std::optional<shop_purchase>
			{
				auto results = db.get_database<Type>()->operator()(
					sqlpp::select(sqlpp::all_of(shop_purchase::table))
							.from(shop_purchase::table)
								.where(shop_purchase::table.f_user_id == user_id &&
									   shop_purchase::table.product_type >= item_type_beg && shop_purchase::table.product_type < item_type_end)
									.order_by(shop_purchase::table.purchase_date.desc()).limit(1u));

				if (results.empty())
				{
					return {};
				}

				return shop_purchase(results.front());
			});
		}

		template <database_type_t Type>
		void delete_player_data(const std::uint64_t user_id)
		{
			return database::access([&](database_t& db)
			{
				db.get_database<Type>()->operator()(
					sqlpp::remove_from(shop_purchase::table)
						.where(shop_purchase::table.f_user_id == user_id));
			});
		}
	}

	bool add_entry(const std::uint64_t user_id, const entry_params_t& params)
	{
		RUN_IMPL(impl::add_entry, user_id, params);
	}

	bool add_product(const std::uint64_t user_id, const shop_product_t& product, const std::uint32_t purchase_quantity)
	{
		entry_params_t params{};
		params.product_type = product.product_id;
		params.purchase_quantity = purchase_quantity;
		params.coin_quantity = product.price;
		params.event_type = event_loss;
		params.item = product.item;
		params.lang_id = product.lang_id;

		return add_entry(user_id, params);
	}

	bool add_spent(const std::uint64_t user_id, const std::uint64_t lang_id, const std::uint32_t coin_quantity)
	{
		entry_params_t params{};
		params.product_type = 0;
		params.purchase_quantity = 1;
		params.coin_quantity = coin_quantity;
		params.event_type = event_loss;
		params.lang_id = lang_id;

		return add_entry(user_id, params);
	}

	bool add_spent(const std::uint64_t user_id, const std::uint64_t lang_id, const std::uint32_t coin_quantity, const game::item_t& item)
	{
		entry_params_t params{};
		params.product_type = 0;
		params.purchase_quantity = 1;
		params.coin_quantity = coin_quantity;
		params.event_type = event_loss;
		params.lang_id = lang_id;
		params.item = item;

		return add_entry(user_id, params);
	}

	bool add_received_coin(const std::uint64_t user_id, const std::uint64_t lang_id, const std::uint32_t coin_quantity)
	{
		entry_params_t params{};
		params.product_type = 0;
		params.purchase_quantity = 1;
		params.coin_quantity = coin_quantity;
		params.event_type = event_gain;
		params.lang_id = lang_id;
		params.item.category = game::ITEM_CATEGORY_COIN;

		return add_entry(user_id, params);
	}

	std::size_t get_history_size(const std::uint64_t user_id)
	{
		RUN_IMPL(impl::get_history_size, user_id);
	}

	std::vector<shop_purchase> get_history(const std::uint64_t user_id, const std::uint32_t offset, const std::uint32_t limit)
	{
		RUN_IMPL(impl::get_history, user_id, offset, limit);
	}

	std::optional<shop_purchase> get_last_item_purchase(const std::uint64_t user_id, const std::uint32_t item_type)
	{
		RUN_IMPL(impl::get_last_item_purchase, user_id, item_type);
	}

	std::optional<shop_purchase> get_last_item_purchase_range(const std::uint64_t user_id, const std::uint32_t item_type_beg, const std::uint32_t item_type_end)
	{
		RUN_IMPL(impl::get_last_item_purchase_range, user_id, item_type_beg, item_type_end);
	}

	std::unordered_map<std::uint32_t, std::size_t> get_purchase_counts(const std::uint64_t user_id)
	{
		RUN_IMPL(impl::get_purchase_counts, user_id);
	}

	std::size_t get_purchase_count(const std::uint64_t user_id, const std::uint32_t product_type)
	{
		RUN_IMPL(impl::get_purchase_count, user_id, product_type);
	}

	void delete_player_data(const std::uint64_t user_id)
	{
		RUN_IMPL(impl::delete_player_data, user_id);
	}

	json::value purchase_product(const users::user& user, const std::uint32_t product_type, const std::function<bool(json::value& result)>& callback)
	{
		json::value result;

		const auto product = find_product(product_type);
		if (!product.has_value())
		{
			result["result"] = game::error_map[ERR_NOT_FOUND];
			return result;
		}

		return purchase_product(user, product.value(), callback);
	}

	json::value purchase_product(const users::user& user, const shop_product_t& product, const std::function<bool(json::value& result)>& callback)
	{
		json::value result;

		result["purchase_result"]["is_coin"] = 0;
		result["purchase_result"]["payment"] = 0;
		result["purchase_result"]["balance"] = 0;

		if (!user.spend_sv_coins(product.price))
		{
			result["result"] = game::error_map[ERR_MBCOIN_SHORTAGE];
			return result;
		}

		if (!add_product(user.get_user_id(), product) || !callback(result))
		{
			user.add_sv_coins(product.price);
			result["result"] = game::error_map[ERR_DATABASE];
			return result;
		}

		result["purchase_result"]["is_coin"] = 1;
		result["purchase_result"]["payment"] = product.price;
		result["purchase_result"]["balance"] = user.get_sv_coin() - product.price;

		return result;
	}

	class table final : public table_interface
	{
	public:
		void create(database_t& database) override
		{
			database.run_query("mgstpp.shop_purchases.create");

			get_shop_product_list();
			get_shop_product_map();
		}
	};
}

REGISTER_TABLE(database::shop_purchases::table, 2)
