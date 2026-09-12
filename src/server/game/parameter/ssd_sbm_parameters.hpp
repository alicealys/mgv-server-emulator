#pragma once

#include "base_parameter.hpp"

namespace game::parameters
{
	class ssd_sbm_parameters final : public base_parameter
	{
	public:
		ssd_sbm_parameters();
		bool parse(nlohmann::json& data) override;

		std::unordered_map<std::uint32_t, std::shared_ptr<production_t>> productions;
		std::unordered_map<std::uint32_t, std::shared_ptr<recipe_t>> recipes;
		std::unordered_map<std::uint32_t, std::shared_ptr<survival_gear_t>> survival_gears;
		std::array<gradeup_spec_t, 255> gradeup_spec;

	};
}
