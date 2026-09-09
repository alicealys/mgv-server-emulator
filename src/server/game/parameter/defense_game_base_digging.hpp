#pragma once

#include "base_parameter.hpp"

namespace game::parameters
{
	class defense_game_base_digging final : public base_parameter
	{
	public:
		defense_game_base_digging();
		bool parse(nlohmann::json& data) override;
	};
}
