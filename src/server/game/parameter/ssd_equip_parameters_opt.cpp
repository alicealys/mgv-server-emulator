#include <std_include.hpp>

#include "ssd_equip_parameters_opt.hpp"

namespace game::parameters
{
	ssd_equip_parameters_opt::ssd_equip_parameters_opt()
	{
		this->load("SsdEquipParameters_opt");
	}

	bool ssd_equip_parameters_opt::parse(const nlohmann::json& data)
	{
		return true;
	}
}
