#include <std_include.hpp>

#include "ssd_buff_parameter_debug.hpp"

namespace game::parameters
{
	ssd_buff_parameter_debug::ssd_buff_parameter_debug()
	{
		this->load("SsdBuffParameter_debug");
	}

	bool ssd_buff_parameter_debug::parse(nlohmann::json& data)
	{
		return true;
	}
}
