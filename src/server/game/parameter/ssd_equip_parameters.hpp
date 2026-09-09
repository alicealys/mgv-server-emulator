#pragma once

#include "base_parameter.hpp"

namespace game::parameters
{
	class ssd_equip_parameters final : public base_parameter
	{
	public:
		ssd_equip_parameters();
		bool parse(nlohmann::json& data) override;
	};
}
