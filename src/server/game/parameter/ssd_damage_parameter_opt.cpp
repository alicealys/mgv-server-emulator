#include <std_include.hpp>

#include "ssd_damage_parameter_opt.hpp"

namespace game::parameters
{
	ssd_damage_parameter_opt::ssd_damage_parameter_opt()
	{
		this->load("SsdDamageParameter_opt");
	}

	bool ssd_damage_parameter_opt::parse(nlohmann::json& data)
	{
		return true;
	}
}
