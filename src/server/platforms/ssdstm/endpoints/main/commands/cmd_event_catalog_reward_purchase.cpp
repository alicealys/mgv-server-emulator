#include <std_include.hpp>

#include "cmd_event_catalog_reward_purchase.hpp"

// not implemented
namespace emulator::ssd
{
	json::value cmd_event_catalog_reward_purchase::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		std::uint32_t catalog_id{};
		if (!json::read(catalog_id, data["catalog_id"]))
		{
			return error(ERR_INVALIDARG);
		}

		result["remaining_point"] = 0;
		result["present_id"] = 0;
		result["result"] = "ERR_NOTIMPLEMENTED";

		return result;
	}

	std::uint32_t cmd_event_catalog_reward_purchase::flags()
	{
		return CMD_NEEDS_USER;
	}
}
