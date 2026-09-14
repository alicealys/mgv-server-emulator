#pragma once

#include "base_parameter.hpp"

namespace game::parameters
{
	class ssd_building_parameter_table final : public base_parameter
	{
	public:
		ssd_building_parameter_table();
		bool parse(json::value& data) override;
	};
}
