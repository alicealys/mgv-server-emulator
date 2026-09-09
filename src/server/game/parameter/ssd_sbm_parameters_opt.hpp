#pragma once

#include "base_parameter.hpp"

namespace game::parameters
{
	class ssd_sbm_parameters_opt : public base_parameter
	{
	public:
		ssd_sbm_parameters_opt();
		bool parse(const nlohmann::json& data) override;
	};
}
