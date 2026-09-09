#pragma once

#include "base_parameter.hpp"

namespace game::parameters
{
	class ssd_replay_mission_info_opt final : public base_parameter
	{
	public:
		ssd_replay_mission_info_opt();
		bool parse(const nlohmann::json& data) override;
	};
}
