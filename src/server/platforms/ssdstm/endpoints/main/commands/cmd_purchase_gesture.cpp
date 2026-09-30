#include <std_include.hpp>

#include "cmd_purchase_gesture.hpp"

#include "database/models/shop_purchases.hpp"

namespace emulator::ssd
{
	json::value cmd_purchase_gesture::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		std::uint8_t id{};
		if (!json::read(id, data["id"]) || id > 128)
		{
			return error(ERR_INVALIDARG);
		}

		const auto product_id = database::shop_purchases::product_gesture_begin + id;
		const auto product = database::shop_purchases::find_product(product_id);
		if (!product.has_value() || product->item.category != game::ITEM_CATEGORY_GESTURE)
		{
			return error(ERR_NOT_FOUND);
		}

		const auto user_inventory = std::make_unique<database::users::user_inventory_t>();
		user->get_inventory(*user_inventory);

		return database::shop_purchases::purchase_product(user.value(), product.value(), [&](json::value& result)
		{
			user_inventory->set_obtained_gesture(id);
			return user->set_inventory(*user_inventory);
		});
	}

	std::uint32_t cmd_purchase_gesture::flags()
	{
		return CMD_NEEDS_USER;
	}
}
