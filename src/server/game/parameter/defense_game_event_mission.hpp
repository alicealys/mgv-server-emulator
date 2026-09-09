#pragma once

#include "base_parameter.hpp"

namespace game::parameters
{
	class defense_game_event_mission final : public base_parameter
	{
	public:
		defense_game_event_mission();
		bool parse(const nlohmann::json& data) override;
	};
}
