#include <std_include.hpp>

#include "cmd_purchase_player_slot.hpp"

#include "database/models/deployments.hpp"
#include "database/models/shop_purchases.hpp"

namespace emulator::ssd
{
	json::value cmd_purchase_player_slot::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		const auto player_count = static_cast<std::uint32_t>(database::players::get_player_count(user->get_user_id()));
		if (player_count >= database::players::max_player_count)
		{
			return error(ERR_OVER_CAPACITY);
		}

		const auto product_type = database::shop_purchases::product_additional_slot_avatar + player_count;
		return database::shop_purchases::purchase_product(user.value(), product_type, [&](json::value& result)
		{
			return user->inc_player_capacity();
		});
	}

	std::uint32_t cmd_purchase_player_slot::flags()
	{
		return CMD_NEEDS_USER;
	}
}
