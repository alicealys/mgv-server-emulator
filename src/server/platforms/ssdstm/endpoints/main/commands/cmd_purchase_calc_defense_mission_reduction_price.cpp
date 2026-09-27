#include <std_include.hpp>

#include "cmd_purchase_calc_defense_mission_reduction_price.hpp"

namespace emulator::ssd
{
	json::value cmd_purchase_calc_defense_mission_reduction_price::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		result["price_result"]["price"] = 0;
		result["result"] = "ERR_NOTIMPLEMENTED";

		return result;
	}

	std::uint32_t cmd_purchase_calc_defense_mission_reduction_price::flags()
	{
		return CMD_NEEDS_USER;
	}
}
