#pragma once

#include "base_parameter.hpp"

namespace game::parameters
{
	class defense_game_quest : public base_parameter
	{
	public:
		defense_game_quest();
		bool parse(const nlohmann::json& data) override;
	};
}
