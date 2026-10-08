#include <std_include.hpp>

#include "ssd_ui_coop_mission_info.hpp"

namespace game::parameters
{
	ssd_ui_coop_mission_info::ssd_ui_coop_mission_info()
	{
		this->load("SsdUiCoopMissionInfo");
	}

	bool ssd_ui_coop_mission_info::parse(json::value& data)
	{
		for (auto i = 0ull; i < data["coopMissionInfo"].size(); i++)
		{
			const auto mission = std::make_shared<game::coop_mission_info_t>();
			if (!mission->parse(data["coopMissionInfo"][i]))
			{
				continue;
			}

			this->coop_missions.insert(std::make_pair(mission->mission_id, mission));
		}

		return true;
	}
}
