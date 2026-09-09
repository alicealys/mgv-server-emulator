#pragma once

#include "base_parameter.hpp"

namespace game::parameters
{
	class ssd_equip_parameters_opt final : public base_parameter
	{
	public:
		ssd_equip_parameters_opt();
		bool parse(nlohmann::json& data) override;
	};
}
