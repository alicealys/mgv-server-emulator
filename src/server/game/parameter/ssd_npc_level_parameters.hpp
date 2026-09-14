#pragma once

#include "base_parameter.hpp"

namespace game::parameters
{
	class ssd_npc_level_parameters final : public base_parameter
	{
	public:
		ssd_npc_level_parameters();
		bool parse(json::value& data) override;
	};
}
