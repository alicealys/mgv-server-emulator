#include <std_include.hpp>

#include "cmd_craft_open_perk.hpp"

namespace emulator::ssd
{
	json::value cmd_craft_open_perk::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		param_t param{};
		if (json::read(param, data))
		{
			return error(ERR_INVALIDARG);
		}

		result["perk_id"] = 0;
        result["result"] = "ERR_NOTIMPLEMENTED";

        return result;
	}

	std::uint32_t cmd_craft_open_perk::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
