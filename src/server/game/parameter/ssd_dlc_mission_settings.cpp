#include <std_include.hpp>

#include "ssd_dlc_mission_settings.hpp"

namespace game::parameters
{
	ssd_dlc_mission_settings::ssd_dlc_mission_settings()
	{
		this->load("SsdDlcMissionSettings");
	}

	bool ssd_dlc_mission_settings::parse(const nlohmann::json& data)
	{
		return true;
	}
}
