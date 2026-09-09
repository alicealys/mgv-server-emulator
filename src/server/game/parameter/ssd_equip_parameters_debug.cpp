#include <std_include.hpp>

#include "ssd_equip_parameters_debug.hpp"

namespace game::parameters
{
	ssd_equip_parameters_debug::ssd_equip_parameters_debug()
	{
		this->load("SsdEquipParameters_debug");
	}

	bool ssd_equip_parameters_debug::parse(nlohmann::json& data)
	{
		return true;
	}
}
