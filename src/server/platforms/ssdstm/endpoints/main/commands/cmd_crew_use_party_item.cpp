#include <std_include.hpp>

#include "cmd_crew_use_party_item.hpp"

namespace emulator::ssd
{
	json::value cmd_crew_use_party_item::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		std::uint32_t item_id{};
		if (json::read(item_id, data["item_id"]))
		{
			return error(ERR_INVALIDARG);
		}

		result["result"] = "ERR_NOTIMPLEMENTED";

		return result;
	}

	std::uint32_t cmd_crew_use_party_item::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
