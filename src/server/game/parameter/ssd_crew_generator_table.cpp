#include <std_include.hpp>

#include "ssd_crew_generator_table.hpp"

namespace game::parameters
{
	ssd_crew_generator_table::ssd_crew_generator_table()
	{
		this->load("SsdCrewGeneratorTable");
	}

	bool ssd_crew_generator_table::parse(const nlohmann::json& data)
	{
		return true;
	}
}
