#include <std_include.hpp>

#include "ssd_base_defense_settings.hpp"

namespace game::parameters
{
	ssd_base_defense_settings::ssd_base_defense_settings()
	{
		this->load("SsdBaseDefenseSettings");
	}

	bool ssd_base_defense_settings::parse(nlohmann::json& data)
	{
		return true;
	}
}
