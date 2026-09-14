#pragma once

#include "base_parameter.hpp"

namespace game::parameters
{
	class ssd_buff_parameter final : public base_parameter
	{
	public:
		ssd_buff_parameter();
		bool parse(glz::json& data) override;
	};
}
