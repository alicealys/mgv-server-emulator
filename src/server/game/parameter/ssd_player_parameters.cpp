#include <std_include.hpp>

#include "ssd_player_parameters.hpp"

namespace game::parameters
{
	ssd_player_parameters::ssd_player_parameters()
	{
		this->load("SsdPlayerParameters");
	}

	bool ssd_player_parameters::parse(nlohmann::json& data)
	{
		return true;
	}
}
