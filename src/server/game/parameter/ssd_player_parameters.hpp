#pragma once

#include "base_parameter.hpp"

namespace game::parameters
{
	class ssd_player_parameters : public base_parameter
	{
	public:
		ssd_player_parameters();
		bool parse(const nlohmann::json& data) override;
	};
}
