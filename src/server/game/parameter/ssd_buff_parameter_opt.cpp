#include <std_include.hpp>

#include "ssd_buff_parameter_opt.hpp"

namespace game::parameters
{
	ssd_buff_parameter_opt::ssd_buff_parameter_opt()
	{
		this->load("SsdBuffParameter_opt");
	}

	bool ssd_buff_parameter_opt::parse(nlohmann::json& data)
	{
		return true;
	}
}
