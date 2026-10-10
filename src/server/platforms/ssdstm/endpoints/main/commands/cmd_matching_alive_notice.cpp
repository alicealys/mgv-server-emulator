#include <std_include.hpp>

#include "cmd_matching_alive_notice.hpp"

#include "database/models/matching.hpp"

namespace emulator::ssd
{
	json::value cmd_matching_alive_notice::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		return {};
	}

	std::uint32_t cmd_matching_alive_notice::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
