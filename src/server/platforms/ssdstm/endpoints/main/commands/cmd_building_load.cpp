#include <std_include.hpp>

#include "cmd_building_load.hpp"

namespace emulator::ssd
{
	json::value cmd_building_load::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		const auto building_info = std::make_unique<database::players::building_info_t>();
		user->current_player->get_building_info(*building_info);

		building_info->to_json(result["cell_info"]);
		building_info->to_json_farming(result["farming_info"]);
		result["defense_mission_disconnect"] = 0;

		return result;
	}

	std::uint32_t cmd_building_load::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
