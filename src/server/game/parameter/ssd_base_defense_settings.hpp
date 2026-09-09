#pragma once

#include "base_parameter.hpp"

namespace game::parameters
{
	class ssd_base_defense_settings final : public base_parameter
	{
	public:
		ssd_base_defense_settings();
		bool parse(const nlohmann::json& data) override;
	};
}
