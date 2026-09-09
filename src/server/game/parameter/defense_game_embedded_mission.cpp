#include <std_include.hpp>

#include "defense_game_embedded_mission.hpp"

namespace game::parameters
{
	defense_game_embedded_mission::defense_game_embedded_mission()
	{
		this->load("DefenseGameEmbeddedMission");
	}

	bool defense_game_embedded_mission::parse(const nlohmann::json& data)
	{
		return true;
	}
}
