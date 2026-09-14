#pragma once

#include "base_parameter.hpp"

namespace game::parameters
{
	class ssd_dlc_mission_settings final : public base_parameter
	{
	public:
		ssd_dlc_mission_settings();
		bool parse(json::value& data) override;
	};
}
