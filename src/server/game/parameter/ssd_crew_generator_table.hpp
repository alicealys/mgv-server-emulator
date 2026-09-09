#pragma once

#include "base_parameter.hpp"

namespace game::parameters
{
	class ssd_crew_generator_table final : public base_parameter
	{
	public:
		ssd_crew_generator_table();
		bool parse(const nlohmann::json& data) override;
	};
}
