#include <std_include.hpp>

#include "cmd_purchase_defense_mission_reduction.hpp"

#include "database/models/defense_missions.hpp"
#include "database/models/shop_purchases.hpp"

namespace emulator::ssd
{
	json::value cmd_purchase_defense_mission_reduction::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		std::uint32_t client_time{};
		if (!json::read(client_time, data["client_time"]))
		{
			return error(ERR_INVALIDARG);
		}

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
		const auto price = game::calc_time_reduction_cost(static_cast<std::uint32_t>(diff.count()));

		auto product = database::shop_purchases::find_product(database::shop_purchases::product_defense_mission_reduction);
		if (!product.has_value())
		{
			return error(ERR_DATABASE);
		}

		product->price = price;

		return database::shop_purchases::purchase_product(user.value(), product.value(), [&](json::value& result)
		{
			return mission->update(mission->get_result(), mission->get_current_wave(), 
				mission->get_clear_rank(), mission->get_total_score(), mission->get_end_date(), now_s);
		});
	}

	std::uint32_t cmd_purchase_defense_mission_reduction::flags()
	{
		return CMD_NEEDS_USER;
	}
}
