#pragma once

#include "base_parameter.hpp"

namespace game::parameters
{
	class defense_game_embedded_mission final : public base_parameter
	{
	public:
		defense_game_embedded_mission();
		bool parse(nlohmann::json& data) override;
	};
}
