#pragma once

#include "base_parameter.hpp"

namespace game::parameters
{
	class ssd_sbm_parameters : public base_parameter
	{
	public:
		ssd_sbm_parameters();
		bool parse(const nlohmann::json& data) override;
	};
}
