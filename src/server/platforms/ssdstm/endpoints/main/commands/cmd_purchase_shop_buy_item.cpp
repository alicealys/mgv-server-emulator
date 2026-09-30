#include <std_include.hpp>

#include "cmd_purchase_shop_buy_item.hpp"

#include "database/models/shop_purchases.hpp"
#include "database/models/present_box.hpp"

namespace emulator::ssd
{
	json::value cmd_purchase_shop_buy_item::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		std::uint32_t id{};
		if (!json::read(id, data["id"]))
		{
			return error(ERR_INVALIDARG);
		}

		const auto product = database::shop_purchases::find_product(id);
		if (!product.has_value())
		{
			return error(ERR_NOT_FOUND);
		}

		const auto purchase_count = database::shop_purchases::get_purchase_count(user->get_user_id(), product->product_id);
		if (purchase_count > product->limit_count)
		{
			return error(ERR_OVER_CAPACITY);
		}

		return database::shop_purchases::purchase_product(user.value(), product.value(), [&](json::value& result)
		{
			database::users::user_inventory_t user_inventory{};
			user->get_inventory(user_inventory);

			database::users::give_item_params_t give_params{};
			give_params.user_inventory = &user_inventory;

			result["present_id"] = 0;

			json::value empty;
			if (user->give_item(product->item, give_params, empty))
			{
				return user->set_inventory(user_inventory);
			}

			const auto expire_date = std::chrono::system_clock::now() + database::shop_purchases::purchase_duration;
			const auto expire_date_s = std::chrono::duration_cast<std::chrono::seconds>(expire_date.time_since_epoch());
			const auto present_id = database::present_box::add_item(user->current_player->get_player_id(),
				database::present_box::present_flag_new | database::present_box::present_flag_expire, expire_date_s, product->item);

			if (present_id == 0ull)
			{
				user->add_sv_coins(product->price);
				result["result"] = game::get_error(ERR_DATABASE);
				return false;
			}

			result["present_id"] = present_id;
			return true;
		});
	}

	std::uint32_t cmd_purchase_shop_buy_item::flags()
	{
		return CMD_NEEDS_USER;
	}
}
