#include <std_include.hpp>

#include "cmd_mission_defense_mission_start.hpp"

namespace emulator::ssd
{
	json::value cmd_mission_defense_mission_start::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		database::players::defense_mission_info_t info{};
		user->current_player->get_defense_mission_info(info);

		if (!info.parameter.parse(data["defense_mission_parameter"]))
		{
			return error(ERR_INVALIDARG);
		}

		user->current_player->set_defense_mission_info(info);

        return result;
	}

	std::uint32_t cmd_mission_defense_mission_start::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
