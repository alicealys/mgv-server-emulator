#include <std_include.hpp>

#include "cmd_steam_shop_approve.hpp"

namespace emulator::ssd
{
	json::value cmd_steam_shop_approve::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		return result;
	}

	std::uint32_t cmd_steam_shop_approve::flags()
	{
		return CMD_NEEDS_USER;
	}
}
