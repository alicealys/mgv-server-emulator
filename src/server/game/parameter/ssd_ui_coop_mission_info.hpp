#pragma once

#include "base_parameter.hpp"

namespace game::parameters
{
	class ssd_ui_coop_mission_info final : public base_parameter
	{
	public:
		ssd_ui_coop_mission_info();
		bool parse(const nlohmann::json& data) override;
	};
}
