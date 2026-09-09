#pragma once

#include "base_parameter.hpp"

namespace game::parameters
{
	class defense_game_base_digging : public base_parameter
	{
	public:
		defense_game_base_digging();
		bool parse(const nlohmann::json& data) override;
	};
}
