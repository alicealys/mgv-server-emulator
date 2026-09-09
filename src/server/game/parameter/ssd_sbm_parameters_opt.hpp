#pragma once

#include "base_parameter.hpp"

namespace game::parameters
{
	class ssd_sbm_parameters_opt final : public base_parameter
	{
	public:
		ssd_sbm_parameters_opt();
		bool parse(nlohmann::json& data) override;
	};
}
