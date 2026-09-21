#include <std_include.hpp>

#include "ssd_base_defense_settings.hpp"

namespace game::parameters
{
	ssd_base_defense_settings::ssd_base_defense_settings()
	{
		this->load("SsdBaseDefenseSettings");
	}

	bool ssd_base_defense_settings::parse(json::value& data)
	{
		for (auto i = 0ull; i < data["missionSettings"].size(); i++)
		{
			auto mission = std::make_shared<defense_mission_settings_t>();
			mission->parse(data["missionSettings"][i]);
			this->mission_settings[mission->mission_id] = mission;
		}

		return true;
	}
}
