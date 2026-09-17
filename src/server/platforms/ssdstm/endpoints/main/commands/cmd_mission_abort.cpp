#include <std_include.hpp>

#include "cmd_mission_abort.hpp"

namespace emulator::ssd
{
	json::value cmd_mission_abort::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

        return result;
	}

	std::uint32_t cmd_mission_abort::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
