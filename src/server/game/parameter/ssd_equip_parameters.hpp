#pragma once

#include "base_parameter.hpp"

namespace game::parameters
{
	class ssd_equip_parameters : public base_parameter
	{
	public:
		ssd_equip_parameters();
		bool parse(const nlohmann::json& data) override;
	};
}
