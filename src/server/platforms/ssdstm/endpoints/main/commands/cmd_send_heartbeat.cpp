#include <std_include.hpp>

#include "cmd_send_heartbeat.hpp"

namespace emulator::ssd
{
	json::value cmd_send_heartbeat::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		return result;
	}

	std::uint32_t cmd_send_heartbeat::flags()
	{
		return CMD_NEEDS_USER;
	}
}
