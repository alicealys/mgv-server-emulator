#include <std_include.hpp>

#include "cmd_building_mining_machine_repair.hpp"

namespace emulator::ssd
{
	json::value cmd_building_mining_machine_repair::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		auto& option_j = data["option"];
		auto& damage_rate_j = data["damage_rate"];

		if (!option_j.is_uint64() || !damage_rate_j.is_uint64())
		{
			return error(ERR_INVALIDARG);
		}

		database::players::defense_mission_info_t initial_info{};
		initial_info.initialize();

		database::players::defense_mission_info_t info{};
		user->current_player->get_defense_mission_info(info);
		info.status.mining_machine_life = initial_info.status.mining_machine_life;
		user->current_player->set_defense_mission_info(info);

		return result;
	}

	std::uint32_t cmd_building_mining_machine_repair::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
