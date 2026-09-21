#include <std_include.hpp>

#include "cmd_crew_use_item.hpp"

namespace emulator::ssd
{
	json::value cmd_crew_use_item::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		std::uint32_t unique_id{};
		if (!json::read(unique_id, data["unique_id"]))
		{
			return error(ERR_INVALIDARG);
		}

		auto& list_j = data["item_list"];
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

	std::uint32_t cmd_crew_use_item::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
