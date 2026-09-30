#include <std_include.hpp>

#include "cmd_purchase_additional_storage.hpp"

#include "database/models/shop_purchases.hpp"

namespace emulator::ssd
{
	json::value cmd_purchase_additional_storage::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;
		
		std::uint8_t additional_storage_type{};
		if (!json::read(additional_storage_type, data["additional_storage_type"]) || additional_storage_type >= 2)
		{
			return error(ERR_INVALIDARG);
		}

		auto product_type = 0u;
		switch (additional_storage_type)
		{
		case 0:
			product_type = database::shop_purchases::product_increase_storage_limit_weapons;
			break;
		case 1:
			product_type = database::shop_purchases::product_increase_storage_limit_gear;
			break;
		}

		const auto count = database::shop_purchases::get_purchase_count(user->get_user_id(), product_type);
		if (count >= 3)
		{
			return error(ERR_OVER_CAPACITY);
		}

		return database::shop_purchases::purchase_product(user.value(), product_type, [&](json::value& result)
		{
			result["additional_storage_type"] = additional_storage_type;
			return true;
		});
	}

	std::uint32_t cmd_purchase_additional_storage::flags()
	{
		return CMD_NEEDS_USER;
	}
}
