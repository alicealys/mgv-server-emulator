#include <std_include.hpp>

#include "ssd_sbm_parameters.hpp"

namespace game::parameters
{
	ssd_sbm_parameters::ssd_sbm_parameters()
	{
		this->load("SsdSbmParameters");
	}

	bool ssd_sbm_parameters::parse(const nlohmann::json& data)
	{
		return true;
	}
}
