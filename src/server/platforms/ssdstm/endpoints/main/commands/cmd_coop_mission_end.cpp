#include <std_include.hpp>

#include "cmd_coop_mission_end.hpp"

// not implemented
namespace emulator::ssd
{
	json::value cmd_coop_mission_end::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;
		result["result"] = "ERR_NOTIMPLEMENTED";
		return result;
	}

	std::uint32_t cmd_coop_mission_end::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
