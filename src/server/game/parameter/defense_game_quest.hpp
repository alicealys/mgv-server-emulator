#pragma once

#include "base_parameter.hpp"

namespace game::parameters
{
	class defense_game_quest final : public base_parameter
	{
	public:
		defense_game_quest();
		bool parse(nlohmann::json& data) override;
	};
}
