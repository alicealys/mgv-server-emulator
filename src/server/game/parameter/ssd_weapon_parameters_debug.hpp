#pragma once

#include "base_parameter.hpp"

namespace game::parameters
{
	class ssd_weapon_parameters_debug final : public base_parameter
	{
	public:
		ssd_weapon_parameters_debug();
		bool parse(const nlohmann::json& data) override;
	};
}
