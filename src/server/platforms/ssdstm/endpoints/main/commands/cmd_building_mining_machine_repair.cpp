#include <std_include.hpp>

#include "cmd_building_mining_machine_repair.hpp"

namespace emulator::ssd
{
	glz::json cmd_building_mining_machine_repair::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;

		auto& option_j = data["option"];
		auto& damage_rate_j = data["damage_rate"];

		if (!option_j.is_uint64() || !damage_rate_j.is_uint64())
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
