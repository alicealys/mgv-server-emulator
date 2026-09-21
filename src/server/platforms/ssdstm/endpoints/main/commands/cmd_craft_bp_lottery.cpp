#include <std_include.hpp>

#include "cmd_craft_bp_lottery.hpp"

namespace emulator::ssd
{
	json::value cmd_craft_bp_lottery::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		auto& list_j = data["battle_pack_list"];
		if (!list_j.is_array())
		{
			return error(ERR_INVALIDARG);
		}

		for (auto i = 0ull; i < list_j.size(); i++)
		{
			entry_t entry{};
			if (!json::read(entry, list_j[i]))
			{
				continue;
			}

			// TODO
		}

		result["battle_pack_lottery_list"]["present_list"] = json::array();
		result["battle_pack_lottery_list"]["expire_date"] = 0;
		result["battle_pack_lottery_list"]["resources_list"] = json::array();
		result["battle_pack_lottery_list"]["stackable_list"] = json::array();
		result["battle_pack_lottery_list"]["nonstackable_list"] = json::array();
		result["battle_pack_lottery_list"]["energy"] = 0;
		result["result"] = "ERR_NOTIMPLEMENTED";

        return result;
	}

	std::uint32_t cmd_craft_bp_lottery::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
