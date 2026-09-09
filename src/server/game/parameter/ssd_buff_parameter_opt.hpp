#pragma once

#include "base_parameter.hpp"

namespace game::parameters
{
	class ssd_buff_parameter_opt : public base_parameter
	{
	public:
		ssd_buff_parameter_opt();
		bool parse(const nlohmann::json& data) override;
	};
}
