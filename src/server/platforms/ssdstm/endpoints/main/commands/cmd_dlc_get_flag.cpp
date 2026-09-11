#include <std_include.hpp>

#include "cmd_dlc_get_flag.hpp"

namespace emulator::ssd
{
	nlohmann::json cmd_dlc_get_flag::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;

		result["consumable_count"] = {0, 0, 0, 0, 0, 0, 0, 0};
		result["flag"] = user->get_dlc_flag();

		return result;
	}

	std::uint32_t cmd_dlc_get_flag::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
