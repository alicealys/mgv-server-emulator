#include <std_include.hpp>

#include "cmd_survive_supply_lottery.hpp"

namespace emulator::ssd
{
	json::value cmd_survive_supply_lottery::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		param_t param{};
		if (!json::read(param, data))
		{
			return error(ERR_INVALIDARG);
		}

		return error(ERR_UNKNOWN);
	}

	std::uint32_t cmd_survive_supply_lottery::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
