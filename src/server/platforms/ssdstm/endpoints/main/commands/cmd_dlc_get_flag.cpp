#include <std_include.hpp>

#include "cmd_dlc_get_flag.hpp"

namespace emulator::ssd
{
	json::value cmd_dlc_get_flag::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		result["consumable_count"] = json::value::array_t{0, 0, 0, 0, 0, 0, 0, 0};
		result["flag"] = user->get_dlc_flag();

		return result;
	}

	std::uint32_t cmd_dlc_get_flag::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
