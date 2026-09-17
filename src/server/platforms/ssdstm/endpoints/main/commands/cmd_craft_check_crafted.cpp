#include <std_include.hpp>

#include "cmd_craft_check_crafted.hpp"

namespace emulator::ssd
{
	json::value cmd_craft_check_crafted::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		std::uint16_t inventory_index{};
		if (json::read(inventory_index, data["inventory_index"]))
		{
			return error(ERR_INVALIDARG);
		}

		result["left_resources"] = json::array();
		result["crafted_nonstackable_items"] = json::array();
		result["result"] = "ERR_NOTIMPLEMENTED";

        return result;
	}

	std::uint32_t cmd_craft_check_crafted::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
