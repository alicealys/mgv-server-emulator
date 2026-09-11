#include <std_include.hpp>

#include "cmd_building_load.hpp"

namespace emulator::ssd
{
	nlohmann::json cmd_building_load::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;

		auto& map_location_j = data["map_location"];
		if (!map_location_j.is_number_unsigned())
		{
			return error(ERR_INVALIDARG);
		}

		const auto building_info = std::make_unique<database::players::building_info_t>();
		const auto map_location = map_location_j.get<std::uint32_t>();
		if (map_location > 1)
		{
			return error(ERR_INVALIDARG);
		}

		user->current_player->get_building_info(*building_info, map_location);

		building_info->to_json(result["cell_info"]);
		result["defense_mission_disconnect"] = 0;
		result["farming_info"] = nlohmann::json::array();

		// TODO

        return result;
	}

	std::uint32_t cmd_building_load::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
