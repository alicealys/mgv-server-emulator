#pragma once

#include "base_parameter.hpp"

namespace game::parameters
{
	class ssd_crew_generator_table final : public base_parameter
	{
	public:
		ssd_crew_generator_table();
		bool parse(nlohmann::json& data) override;

		std::unordered_map<std::uint32_t, std::shared_ptr<crew_member_type_t>> crew_member_types;
	};
}
