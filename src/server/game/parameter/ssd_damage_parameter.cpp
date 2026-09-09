#include <std_include.hpp>

#include "ssd_damage_parameter.hpp"

namespace game::parameters
{
	ssd_damage_parameter::ssd_damage_parameter()
	{
		this->load("SsdDamageParameter");
	}

	bool ssd_damage_parameter::parse(nlohmann::json& data)
	{
		return true;
	}
}
