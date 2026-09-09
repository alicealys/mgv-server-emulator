#include <std_include.hpp>

#include "ssd_buff_parameter.hpp"

namespace game::parameters
{
	ssd_buff_parameter::ssd_buff_parameter()
	{
		this->load("SsdBuffParameter");
	}

	bool ssd_buff_parameter::parse(nlohmann::json& data)
	{
		return true;
	}
}
