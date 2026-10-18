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
			return error(ERR_PURCHASE_LIMIT);
		}

		auto product_type = 0u;
		auto count = 0u;

		switch (additional_storage_type)
		{
		case 0:
			product_type = database::shop_purchases::product_increase_storage_limit_weapons;
			count = user->get_additional_storage_01();
			break;
		case 1:
			product_type = database::shop_purchases::product_increase_storage_limit_gear;
			count = user->get_additional_storage_02();
			break;
		}

		if (count >= 3)
		{
			return error(ERR_OVER_CAPACITY);
		}

		return database::shop_purchases::purchase_product(user.value(), product_type, [&](json::value& result)
		{
			result["additional_storage_type"] = additional_storage_type;

			switch (additional_storage_type)
			{
			case 0:
				return user->set_additional_storage_01(count + 1);
			case 1:
				return user->set_additional_storage_02(count + 1);
			}

			return false;
		});
	}

	std::uint32_t cmd_purchase_additional_storage::flags()
	{
		return CMD_NEEDS_USER;
	}
}
