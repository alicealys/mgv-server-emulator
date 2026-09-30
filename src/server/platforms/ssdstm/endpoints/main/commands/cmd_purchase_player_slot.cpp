#include <std_include.hpp>

#include "cmd_purchase_player_slot.hpp"

#include "database/models/deployments.hpp"
#include "database/models/shop_purchases.hpp"

namespace emulator::ssd
{
	json::value cmd_purchase_player_slot::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		const auto players = database::players::get_player_list(user->get_user_id());
		if (players.size() >= database::players::max_player_count)
		{
			return error(ERR_OVER_CAPACITY);
		}

		return database::shop_purchases::purchase_product(user.value(), database::shop_purchases::product_additional_slot_avatar, [&](json::value& result)
		{
			const auto player = database::players::create(user->get_user_id());
			if (!player.has_value())
			{
				return false;
			}

			const auto team_count = database::deployments::get_team_count(user->current_player->get_player_id());
			for (auto i = 1ull; i < team_count; i++)
			{
				database::deployments::create_team(player->get_player_id());
			}

			return true;
		});
	}

	std::uint32_t cmd_purchase_player_slot::flags()
	{
		return CMD_NEEDS_USER;
	}
}
