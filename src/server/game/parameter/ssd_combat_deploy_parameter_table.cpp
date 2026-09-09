#include <std_include.hpp>

#include "ssd_combat_deploy_parameter_table.hpp"

namespace game::parameters
{
	ssd_combat_deploy_parameter_table::ssd_combat_deploy_parameter_table()
	{
		this->load("SsdCombatDeployParameterTable");
	}

	bool ssd_combat_deploy_parameter_table::parse(const nlohmann::json& data)
	{
		return true;
	}
}
