#include <std_include.hpp>

#include "cmd_mission_defense_mission_start.hpp"

namespace emulator::ssd
{
	json::value cmd_mission_defense_mission_start::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		param_t param{};
		if (!json::read(param, data["defense_mission_parameter"]))
		{
			return error(ERR_INVALIDARG);
		}

        result["result"] = "ERR_NOTIMPLEMENTED";

        return result;
	}

	std::uint32_t cmd_mission_defense_mission_start::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
