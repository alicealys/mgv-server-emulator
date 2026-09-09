#include <std_include.hpp>

#include "ssd_weapon_parameters_debug.hpp"

namespace game::parameters
{
	ssd_weapon_parameters_debug::ssd_weapon_parameters_debug()
	{
		this->load("SsdWeaponParameters_debug");
	}

	bool ssd_weapon_parameters_debug::parse(nlohmann::json& data)
	{
		return true;
	}
}
