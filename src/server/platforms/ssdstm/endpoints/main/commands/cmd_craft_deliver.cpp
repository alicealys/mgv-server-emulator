#include <std_include.hpp>

#include "cmd_craft_deliver.hpp"

namespace emulator::ssd
{
	json::value cmd_craft_deliver::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

        param_t param{};
        if (!json::read(param, data))
        {
            return error(ERR_INVALIDARG);
        }

        result["gain_resources"] = json::array();
        result["gain_stackables"] = json::array();
        result["result"] = "ERR_NOTIMPLEMENTED";

        return result;
	}

	std::uint32_t cmd_craft_deliver::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
