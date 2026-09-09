#include <std_include.hpp>

#include "defense_game_event_mission.hpp"

namespace game::parameters
{
	defense_game_event_mission::defense_game_event_mission()
	{
		this->load("DefenseGameEventMission");
	}

	bool defense_game_event_mission::parse(const nlohmann::json& data)
	{
		return true;
	}
}
