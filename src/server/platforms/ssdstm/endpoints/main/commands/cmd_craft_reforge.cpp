#include <std_include.hpp>

#include "cmd_craft_reforge.hpp"

struct param_t
{
	std::uint16_t inventory_index_target;
	std::uint16_t inventory_index_junk;
	std::uint32_t reforge_type;
};

namespace emulator::ssd
{
	json::value cmd_craft_reforge::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		param_t param{};
		if (json::read(param, data))
		{
			return error(ERR_INVALIDARG);
		}

		result["spec"] = 0;
		result["life_max"] = 0;
		result["result"] = "ERR_NOTIMPLEMENTED";

        return result;
	}

	std::uint32_t cmd_craft_reforge::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
