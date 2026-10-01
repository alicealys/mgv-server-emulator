#include <std_include.hpp>

#include "cmd_purchase_deploy_team.hpp"

#include "database/models/deployments.hpp"
#include "database/models/shop_purchases.hpp"

namespace emulator::ssd
{
	json::value cmd_purchase_deploy_team::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		const auto players = database::players::get_player_list(user->get_user_id());
		const auto team_count = database::deployments::get_team_count(user->current_player->get_player_id());

		if (team_count >= database::deployments::max_team_count)
		{
			return error(ERR_OVER_CAPACITY);
		}

		const auto product_type = database::shop_purchases::product_additional_slot_exploration;
		return database::shop_purchases::purchase_product(user.value(), product_type, [&](json::value& result)
		{
			for (auto i = 0ull; i < players.size(); i++)
			{
				const auto other_team_count = database::deployments::get_team_count(players[i].get_player_id());
				if (other_team_count > team_count)
				{
					continue;
				}

				const auto team_id = database::deployments::create_team(players[i].get_player_id());
				const auto result_team = database::deployments::find_team(team_id);
				if (!result_team.has_value())
				{
					return false;
				}
			}

			result["index"] = team_count;
			return true;
		});
	}

	std::uint32_t cmd_purchase_deploy_team::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
