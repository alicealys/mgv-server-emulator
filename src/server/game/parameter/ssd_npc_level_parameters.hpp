#pragma once

#include "base_parameter.hpp"

namespace game::parameters
{
	class ssd_npc_level_parameters : public base_parameter
	{
	public:
		ssd_npc_level_parameters();
		bool parse(const nlohmann::json& data) override;
	};
}
