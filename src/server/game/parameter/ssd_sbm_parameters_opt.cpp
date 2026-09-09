#include <std_include.hpp>

#include "ssd_sbm_parameters_opt.hpp"

namespace game::parameters
{
	ssd_sbm_parameters_opt::ssd_sbm_parameters_opt()
	{
		this->load("SsdSbmParameters_opt");
	}

	bool ssd_sbm_parameters_opt::parse(const nlohmann::json& data)
	{
		return true;
	}
}
