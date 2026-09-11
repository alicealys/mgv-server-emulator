#include <std_include.hpp>

#include "cmd_building_damage.hpp"

namespace emulator::ssd
{
	nlohmann::json cmd_building_damage::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;

		auto& cell_info = data["cell_info"];
		auto& map_location_j = data["map_location"];

		if (!cell_info.is_object() || !map_location_j.is_number_unsigned())
		{
			return error(ERR_INVALIDARG);
		}

		const auto map_location = map_location_j.get<std::uint32_t>();
		if (map_location > 1)
		{
			return error(ERR_INVALIDARG);
		}

		const auto building_info = std::make_unique<database::players::building_info_t>();
		user->current_player->get_building_info(*building_info, map_location);
		building_info->parse(cell_info, true);
		user->current_player->set_building_info(*building_info, map_location);

		return result;
	}

	std::uint32_t cmd_building_damage::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
