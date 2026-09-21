#pragma once

#include "base_parameter.hpp"

namespace game::parameters
{
	class ssd_base_defense_settings final : public base_parameter
	{
	public:
		ssd_base_defense_settings();
		bool parse(json::value& data) override;

		std::unordered_map<std::uint32_t, std::shared_ptr<defense_mission_settings_t>> mission_settings;
	};
}
