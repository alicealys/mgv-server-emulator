#include <std_include.hpp>

#include "cmd_purchase_deploy_team.hpp"

#include "database/models/deployments.hpp"
#include "database/models/shop_purchases.hpp"

namespace emulator::ssd
{
	json::value cmd_purchase_deploy_team::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		const auto players = database::players::get_player_list(user->get_user_id());
		const auto team_count = user->get_team_count();
		if (team_count >= database::deployments::max_team_count)
		{
			return error(ERR_PURCHASE_LIMIT);
		}

		const auto new_team_count = team_count + 1;
		const auto product_type = database::shop_purchases::product_additional_slot_exploration;
		return database::shop_purchases::purchase_product(user.value(), product_type, [&](json::value& result)
		{
			for (auto i = 0ull; i < players.size(); i++)
			{
				const auto other_team_count = database::deployments::get_team_count(players[i].get_player_id());
				for (auto o = other_team_count; o < new_team_count; o++)
				{
					database::deployments::create_team(players[i].get_player_id());
				}
			}

			result["index"] = team_count;
			return user->set_team_count(new_team_count);
		});
	}

	std::uint32_t cmd_purchase_deploy_team::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
