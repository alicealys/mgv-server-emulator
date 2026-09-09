#include <std_include.hpp>

#include "ssd_weapon_parameters_opt.hpp"

namespace game::parameters
{
	ssd_weapon_parameters_opt::ssd_weapon_parameters_opt()
	{
		this->load("SsdWeaponParameters_opt");
	}

	bool ssd_weapon_parameters_opt::parse(nlohmann::json& data)
	{
		return true;
	}
}
