#include <std_include.hpp>

#include "cmd_deploy_load_team.hpp"

#include "database/models/deployments.hpp"
#include "database/models/shop_purchases.hpp"

#include "utils/random.hpp"

namespace emulator::ssd
{
	json::value cmd_deploy_load_team::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		const auto product = database::shop_purchases::find_product(database::shop_purchases::product_additional_slot_exploration);
		const auto slot_price = product.has_value() 
			? product->price
			: 0u;

		for (auto i = 0ull; i < database::deployments::max_team_count; i++)
		{
			if (i < database::deployments::min_team_count)
			{
				result["price_list"][i] = 0;
			}
			else
			{
				result["price_list"][i] = slot_price;
			}
		}

		const auto teams = database::deployments::get_all_teams(user->current_player->get_player_id());
		for (auto i = 0ull; i < teams.size(); i++)
		{
			teams[i].to_json(result["team_list"][i]);
		}

		return result;
	}

	std::uint32_t cmd_deploy_load_team::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
