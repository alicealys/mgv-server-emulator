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

		const auto player_count = static_cast<std::uint32_t>(database::players::get_player_count(user->get_user_id()));
		const auto player_slot_product_type = database::shop_purchases::product_additional_slot_avatar + player_count;
		const auto is_player_slot_purchase = product->product_id == player_slot_product_type && 
			product->item.category == game::ITEM_CATEGORY_CHARACTER_SLOT && product->item.param1 == player_count + 1;

		const auto item = database::shop_purchases::find_item(id);
		if (!item.has_value() && !is_player_slot_purchase)
		{
			return error(ERR_NOT_FOUND);
		}

		const auto user_inventory = std::make_unique<database::users::user_inventory_t>();
		user->get_inventory(*user_inventory);
		auto is_permanent = false;
		if (user->has_permanent_item(product->item, *user_inventory, is_permanent))
		{
			return error(ERR_OVER_CAPACITY);
		}

		if (!is_permanent)
		{
			const auto purchase_count = database::shop_purchases::get_purchase_count(user->get_user_id(), product->product_id);
			if (purchase_count > product->limit_count && product->limit_count != 0)
			{
				return error(ERR_OVER_CAPACITY);
			}
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
				return user->set_inventory(user_inventory) || is_player_slot_purchase;
			}

			const auto expire_date = std::chrono::system_clock::now() + database::shop_purchases::purchase_duration;
			const auto expire_date_s = std::chrono::duration_cast<std::chrono::seconds>(expire_date.time_since_epoch());
			const auto present_id = database::present_box::add_item(user->current_player->get_player_id(),
				database::present_box::present_flag_new | 
				database::present_box::present_flag_expire | 
				database::present_box::present_flag_purchase, 
				expire_date_s, product->item);

			if (present_id == 0ull)
			{
				user->add_sv_coins(product->price);
				result["result"] = game::error_map[ERR_DATABASE];
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
