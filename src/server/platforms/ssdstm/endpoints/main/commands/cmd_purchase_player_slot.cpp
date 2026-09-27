#include <std_include.hpp>

#include "cmd_purchase_player_slot.hpp"

namespace emulator::ssd
{
	json::value cmd_purchase_player_slot::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		result["purchase_result"]["is_coin"] = 0;
		result["purchase_result"]["payment"] = 0;
		result["purchase_result"]["balance"] = 0;
        result["result"] = "ERR_NOTIMPLEMENTED";

        return result;
	}

	std::uint32_t cmd_purchase_player_slot::flags()
	{
		return CMD_NEEDS_USER;
	}
}
