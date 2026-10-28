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
		std::vector<std::shared_ptr<crew_sickness_t>> sick_table;
		std::unordered_map<std::uint32_t, std::shared_ptr<crew_set_table_t>> job_set_table;
		std::unordered_map<std::uint32_t, std::shared_ptr<crew_set_table_t>> skill_set_table;
		std::unordered_map<std::uint32_t, std::shared_ptr<crew_type_t>> type_table;
		std::unordered_map<std::uint32_t, std::shared_ptr<crew_unique_type_t>> unique_table;
		std::unordered_map<std::uint32_t, std::shared_ptr<crew_quest_t>> quest_table;
		std::vector<bool> crew_quest;

		inline bool is_crew_quest(const std::uint32_t index)
		{
			if (index >= this->crew_quest.size())
			{
				return false;
			}

			return this->crew_quest[index];
		}
	};
}
