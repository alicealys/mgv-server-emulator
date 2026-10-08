#pragma once

#include "base_parameter.hpp"

namespace game::parameters
{
	class ssd_ui_coop_mission_info final : public base_parameter
	{
	public:
		ssd_ui_coop_mission_info();
		bool parse(json::value& data) override;

		std::unordered_map<std::uint32_t, std::shared_ptr<coop_mission_info_t>> coop_missions;
	};
}
