#include <std_include.hpp>

#include "defense_game_base_digging.hpp"

namespace game::parameters
{
	defense_game_base_digging::defense_game_base_digging()
	{
		this->load("DefenseGameBaseDigging");
	}

	bool defense_game_base_digging::parse(nlohmann::json& data)
	{
		return true;
	}
}
