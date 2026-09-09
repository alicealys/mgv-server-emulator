#include <std_include.hpp>

#include "cmd_building_load.hpp"

namespace emulator::ssd
{
	nlohmann::json cmd_building_load::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;

		static const auto default_data = utils::resources::load_json(RESOURCE_DEFAULT_DATA);

		result["cell_info"] = default_data["cell_info"];
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
