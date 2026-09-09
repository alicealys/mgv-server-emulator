#include <std_include.hpp>

#include "ssd_walker_gear_parameters.hpp"

namespace game::parameters
{
	ssd_walker_gear_parameters::ssd_walker_gear_parameters()
	{
		this->load("SsdWalkerGearParameters");
	}

	bool ssd_walker_gear_parameters::parse(const nlohmann::json& data)
	{
		return true;
	}
}
