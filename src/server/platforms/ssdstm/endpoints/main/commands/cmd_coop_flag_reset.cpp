#include <std_include.hpp>

#include "cmd_coop_flag_reset.hpp"

namespace emulator::ssd
{
	json::value cmd_coop_flag_reset::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		return result;
	}

	std::uint32_t cmd_coop_flag_reset::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
