#include <std_include.hpp>

#include "ssd_weapon_parameters.hpp"

namespace game::parameters
{
	ssd_weapon_parameters::ssd_weapon_parameters()
	{
		this->load("SsdWeaponParameters");
	}

	bool ssd_weapon_parameters::parse(nlohmann::json& data)
	{
		return true;
	}
}
