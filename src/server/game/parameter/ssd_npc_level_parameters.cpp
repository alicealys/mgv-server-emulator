#include <std_include.hpp>

#include "ssd_npc_level_parameters.hpp"

namespace game::parameters
{
	ssd_npc_level_parameters::ssd_npc_level_parameters()
	{
		this->load("SsdNpcLevelParameters");
	}

	bool ssd_npc_level_parameters::parse(nlohmann::json& data)
	{
		return true;
	}
}
