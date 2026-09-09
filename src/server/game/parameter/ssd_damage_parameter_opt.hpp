#pragma once

#include "base_parameter.hpp"

namespace game::parameters
{
	class ssd_damage_parameter_opt final : public base_parameter
	{
	public:
		ssd_damage_parameter_opt();
		bool parse(const nlohmann::json& data) override;
	};
}
