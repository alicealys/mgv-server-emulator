#pragma once

#include "base_parameter.hpp"

namespace game::parameters
{
	class ssd_walker_gear_parameters final : public base_parameter
	{
	public:
		ssd_walker_gear_parameters();
		bool parse(const nlohmann::json& data) override;
	};
}
