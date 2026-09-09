#include <std_include.hpp>

#include "ssd_ui_coop_mission_info.hpp"

namespace game::parameters
{
	ssd_ui_coop_mission_info::ssd_ui_coop_mission_info()
	{
		this->load("SsdUiCoopMissionInfo");
	}

	bool ssd_ui_coop_mission_info::parse(nlohmann::json& data)
	{
		return true;
	}
}
