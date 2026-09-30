#include <std_include.hpp>

#include "cmd_purchase_shop_get_item_list.hpp"

#include "database/models/shop_purchases.hpp"

namespace emulator::ssd
{
	json::value cmd_purchase_shop_get_item_list::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		auto product_counts = database::shop_purchases::get_purchase_counts(user->get_user_id());
		const auto& products = database::shop_purchases::get_shop_item_list();
		for (auto i = 0ull; i < products.size(); i++)
		{
			auto& entry = result["item_list"][i];

			entry["id"] = products[i].product_id;
			entry["lang_id"] = products[i].lang_id;
			entry["limit_count"] = products[i].limit_count;
			entry["price"] = products[i].price;

			entry["flag"] = 0; //?
			entry["current_count"] = product_counts[products[i].product_id];

			products[i].item.to_json(entry["item"]);
		}

		result["next_date"] = std::time(nullptr) + 14ull * 86400ull; // todo

		return result;
	}

	std::uint32_t cmd_purchase_shop_get_item_list::flags()
	{
		return CMD_NEEDS_USER;
	}
}
