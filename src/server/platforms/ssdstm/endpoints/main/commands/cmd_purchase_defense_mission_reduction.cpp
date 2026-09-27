#include <std_include.hpp>

#include "cmd_purchase_defense_mission_reduction.hpp"

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

		result["result"] = "ERR_NOTIMPLEMENTED";

        return result;
	}

	std::uint32_t cmd_purchase_defense_mission_reduction::flags()
	{
		return CMD_NEEDS_USER;
	}
}
