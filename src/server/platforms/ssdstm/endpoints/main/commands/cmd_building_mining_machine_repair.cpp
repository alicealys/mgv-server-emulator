#include <std_include.hpp>

#include "cmd_building_mining_machine_repair.hpp"

namespace emulator::ssd
{
	nlohmann::json cmd_building_mining_machine_repair::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;

		auto& option_j = data["option"];
		auto& damage_rate_j = data["damage_rate"];

		if (!option_j.is_number_unsigned() || !damage_rate_j.is_number_unsigned())
		{
			return error(ERR_INVALIDARG);
		}

		// TODO

		return result;
	}

	std::uint32_t cmd_building_mining_machine_repair::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
