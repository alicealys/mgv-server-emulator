#include <std_include.hpp>

#include "cmd_craft_invest_to_player.hpp"

#include <utils/json_utils.hpp>

struct param_t
{
	std::uint32_t cls;
	std::uint32_t energy;
};

namespace emulator::ssd
{
	json::value cmd_craft_invest_to_player::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		param_t param{};
		if (json::read(param, data))
		{
			return error(ERR_INVALIDARG);
		}

		std::uint8_t survival_slot_new[2]{};
		if (!utils::json_utils::parse_base64(data["survival_slot_new"], survival_slot_new))
		{
			return error(ERR_INVALIDARG);
		}

		result["energy"] = 0;
		result["result"] = "ERR_NOTIMPLEMENTED";

        return result;
	}

	std::uint32_t cmd_craft_invest_to_player::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
