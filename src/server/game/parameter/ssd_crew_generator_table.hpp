#pragma once

#include "base_parameter.hpp"

namespace game::parameters
{
	class ssd_crew_generator_table final : public base_parameter
	{
	public:
		ssd_crew_generator_table();
		bool parse(json::value& data) override;

		std::shared_ptr<crew_skill_t> find_skill(const std::uint32_t type, const std::uint32_t rank);
		std::uint32_t roll_skill(const std::uint32_t skill_set_table_id);
		std::uint32_t roll_job(const std::uint32_t job_set_table_id);

		std::vector<std::shared_ptr<crew_skill_t>> skill_table;
		std::unordered_map<std::uint32_t, std::shared_ptr<crew_set_table_t>> job_set_table;
		std::unordered_map<std::uint32_t, std::shared_ptr<crew_set_table_t>> skill_set_table;
		std::unordered_map<std::uint32_t, std::shared_ptr<crew_type_t>> type_table;
		std::unordered_map<std::uint32_t, std::shared_ptr<crew_unique_type_t>> unique_table;
	};
}
