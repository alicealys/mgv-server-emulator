#pragma once

#include "base_parameter.hpp"

namespace game::parameters
{
	class ssd_buff_parameter_debug final : public base_parameter
	{
	public:
		ssd_buff_parameter_debug();
		bool parse(nlohmann::json& data) override;
	};
}
