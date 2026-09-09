#include <std_include.hpp>

#include "ssd_damage_parameter_debug.hpp"

namespace game::parameters
{
	ssd_damage_parameter_debug::ssd_damage_parameter_debug()
	{
		this->load("SsdDamageParameter_debug");
	}

	bool ssd_damage_parameter_debug::parse(nlohmann::json& data)
	{
		return true;
	}
}
