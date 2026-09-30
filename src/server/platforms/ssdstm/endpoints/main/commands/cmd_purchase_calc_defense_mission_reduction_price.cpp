#include <std_include.hpp>

#include "cmd_purchase_calc_defense_mission_reduction_price.hpp"

#include "database/models/defense_missions.hpp"
#include "database/models/shop_purchases.hpp"

namespace emulator::ssd
{
	json::value cmd_purchase_calc_defense_mission_reduction_price::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		const auto mission = database::defense_missions::get_current_mission(user->current_player->get_player_id());
		if (!mission.has_value())
		{
			return error(ERR_DEFENSE_MISSION_NOT_IN_MISSION);
		}

		const auto now = std::chrono::system_clock::now();
		const auto now_s = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch());

		if (mission->get_next_wave_date() < now_s)
		{
			return error(ERR_DEFENSE_MISSION_ALREADY_STARTED);
		}

		const auto diff = mission->get_next_wave_date() - now_s;
		result["price_result"]["price"] = game::calc_time_reduction_cost(static_cast<std::uint32_t>(diff.count()));

		return result;
	}

	std::uint32_t cmd_purchase_calc_defense_mission_reduction_price::flags()
	{
		return CMD_NEEDS_USER;
	}
}
