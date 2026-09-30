#include <std_include.hpp>

#include "cmd_purchase_load_out.hpp"

#include "database/models/shop_purchases.hpp"

namespace emulator::ssd
{
	json::value cmd_purchase_load_out::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		const auto loadout_count = user->get_loadout_count();
		if (loadout_count >= database::players::max_loadout_count)
		{
			return error(ERR_OVER_CAPACITY);
		}

		const auto product_type = database::shop_purchases::product_additional_slot_loadout;
		return database::shop_purchases::purchase_product(user.value(), product_type, [&](json::value& result)
		{
			if (!user->set_loadout_count(loadout_count + 1))
			{
				return false;
			}

			result["index"] = loadout_count;
			return true;
		});
	}

	std::uint32_t cmd_purchase_load_out::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
