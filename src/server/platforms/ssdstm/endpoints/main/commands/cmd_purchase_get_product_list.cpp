#include <std_include.hpp>

#include "cmd_purchase_get_product_list.hpp"

#include "database/models/shop_purchases.hpp"

namespace emulator::ssd
{
	json::value cmd_purchase_get_product_list::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		const auto& products = database::shop_purchases::get_shop_product_list();
		for (auto i = 0ull; i < products.size(); i++)
		{
			products[i].to_json(result["product_list"][i]);
		}

		return result;
	}
}
