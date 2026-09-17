#include <std_include.hpp>

#include "cmd_craft_check_crafted_all.hpp"

namespace emulator::ssd
{
	json::value cmd_craft_check_crafted_all::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		result["left_resources"] = json::array();
		result["nonstackable_list"] = json::array();
		result["result"] = "ERR_NOTIMPLEMENTED";

        return result;
	}

	std::uint32_t cmd_craft_check_crafted_all::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
