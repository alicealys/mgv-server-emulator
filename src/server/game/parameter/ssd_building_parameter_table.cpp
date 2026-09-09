#include <std_include.hpp>

#include "ssd_building_parameter_table.hpp"

namespace game::parameters
{
	ssd_building_parameter_table::ssd_building_parameter_table()
	{
		this->load("SsdBuildingParameterTable");
	}

	bool ssd_building_parameter_table::parse(const nlohmann::json& data)
	{
		return true;
	}
}
