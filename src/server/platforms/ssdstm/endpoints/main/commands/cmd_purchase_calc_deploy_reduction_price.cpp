#include <std_include.hpp>

#include "cmd_purchase_calc_deploy_reduction_price.hpp"

#include "database/models/deployments.hpp"
#include "database/models/shop_purchases.hpp"

namespace emulator::ssd
{
	json::value cmd_purchase_calc_deploy_reduction_price::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		std::uint32_t team_index{};
		if (!json::read(team_index, data["team_index"]))
		{
			return error(ERR_INVALIDARG);
		}

		const auto team = database::deployments::find_team_by_index(user->current_player->get_player_id(), team_index);
		if (!team.has_value())
		{
			return error(ERR_NOT_FOUND);
		}

		if (team->get_info_status() != database::deployments::team_status_deploy_progress)
		{
			return error(ERR_ALREADY_COMPLETED);
		}

		const auto now = std::chrono::system_clock::now();
		const auto now_s = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch());

		if (team->get_complete_date() < now_s)
		{
			return error(ERR_ALREADY_COMPLETED);
		}

		const auto diff = team->get_complete_date() - now_s;
		result["price_result"]["price"] = game::calc_time_reduction_cost(static_cast<std::uint32_t>(diff.count()));

		return result;
	}

	std::uint32_t cmd_purchase_calc_deploy_reduction_price::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
