#include <std_include.hpp>

#include "defense_game_quest.hpp"

namespace game::parameters
{
	defense_game_quest::defense_game_quest()
	{
		this->load("DefenseGameQuest");
	}

	bool defense_game_quest::parse(const nlohmann::json& data)
	{
		return true;
	}
}
