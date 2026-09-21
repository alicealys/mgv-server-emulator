#include <std_include.hpp>

#include "cmd_crew_group_transfer.hpp"

namespace emulator::ssd
{
	json::value cmd_crew_group_transfer::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		auto& list_j = data["group_transfer_list"];
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

		result["result"] = "ERR_NOTIMPLEMENTED";

		return result;
	}

	std::uint32_t cmd_crew_group_transfer::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
