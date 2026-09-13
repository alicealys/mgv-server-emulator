#include <std_include.hpp>

#include "ssd_crew_generator_table.hpp"

namespace game::parameters
{
	ssd_crew_generator_table::ssd_crew_generator_table()
	{
		this->load("SsdCrewGeneratorTable");
	}

	bool ssd_crew_generator_table::parse(nlohmann::json& data)
	{
		for (auto i = 0ull; i < data["unique_table"].size(); i++)
		{
			auto crew_member_type = std::make_shared<crew_member_type_t>();
			crew_member_type->parse(data["unique_table"][i]);
			this->crew_member_types[crew_member_type->id] = crew_member_type;
		}

		return true;
	}
}
