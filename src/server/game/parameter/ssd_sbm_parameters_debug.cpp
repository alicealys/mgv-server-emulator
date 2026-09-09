#include <std_include.hpp>

#include "ssd_sbm_parameters_debug.hpp"

namespace game::parameters
{
	ssd_sbm_parameters_debug::ssd_sbm_parameters_debug()
	{
		this->load("SsdSbmParameters_debug");
	}

	bool ssd_sbm_parameters_debug::parse(const nlohmann::json& data)
	{
		return true;
	}
}
