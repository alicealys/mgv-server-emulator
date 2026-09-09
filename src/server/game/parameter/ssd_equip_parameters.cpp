#include <std_include.hpp>

#include "ssd_equip_parameters.hpp"

namespace game::parameters
{
	ssd_equip_parameters::ssd_equip_parameters()
	{
		this->load("SsdEquipParameters");
	}

	bool ssd_equip_parameters::parse(nlohmann::json& data)
	{
		return true;
	}
}
