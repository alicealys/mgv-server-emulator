#pragma once

#include "base_parameter.hpp"

namespace game::parameters
{
	class ssd_damage_parameter final : public base_parameter
	{
	public:
		ssd_damage_parameter();
		bool parse(glz::json& data) override;
	};
}
