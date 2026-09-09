#pragma once

#include "base_parameter.hpp"

namespace game::parameters
{
	class ssd_damage_parameter_debug : public base_parameter
	{
	public:
		ssd_damage_parameter_debug();
		bool parse(const nlohmann::json& data) override;
	};
}
