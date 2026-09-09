#pragma once

#include "base_parameter.hpp"

namespace game::parameters
{
	class ssd_combat_deploy_parameter_table final : public base_parameter
	{
	public:
		ssd_combat_deploy_parameter_table();
		bool parse(const nlohmann::json& data) override;
	};
}
