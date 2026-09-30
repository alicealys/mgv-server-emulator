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

		auto count = 0u;
		const auto add_product = [&](const database::shop_purchases::shop_product_t& product)
		{
			auto& entry = result["item_list"][count++];

			entry["id"] = product.product_id;
			entry["lang_id"] = product.lang_id;
			entry["limit_count"] = product.limit_count;
			entry["price"] = product.price;

			entry["flag"] = 0; //?
			entry["current_count"] = product_counts[product.product_id];

			product.item.to_json(entry["item"]);
		};

		for (auto i = 0ull; i < products.size(); i++)
		{
			add_product(products[i]);
		}

		const auto player_count = database::players::get_player_count(user->get_user_id());
		if (player_count < database::players::max_player_count)
		{
			auto product_type = database::shop_purchases::product_additional_slot_avatar;
			switch (player_count)
			{
			case 1:
				product_type = database::shop_purchases::product_additional_slot_avatar_1;
				break;
			case 2:
				product_type = database::shop_purchases::product_additional_slot_avatar_2;
				break;
			case 3:
				product_type = database::shop_purchases::product_additional_slot_avatar_3;
				break;
			}

			const auto product = database::shop_purchases::find_product(product_type);
			if (product.has_value())
			{
				add_product(product.value());
			}
		}

		result["next_date"] = std::time(nullptr) + 14ull * 86400ull; // todo

		return result;
	}

	std::uint32_t cmd_purchase_shop_get_item_list::flags()
	{
		return CMD_NEEDS_USER;
	}
}
