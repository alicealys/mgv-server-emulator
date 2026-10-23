#include <std_include.hpp>

#include "ssd_crew_generator_table.hpp"

#include "utils/random.hpp"

namespace game::parameters
{
	ssd_crew_generator_table::ssd_crew_generator_table()
	{
		this->load("SsdCrewGeneratorTable");
	}

	bool ssd_crew_generator_table::parse(json::value& data)
	{
		for (auto i = 0ull; i < data["skill_table"].size(); i++)
		{
			auto skill = std::make_shared<crew_skill_t>();
			if (!skill->parse(data["skill_table"][i]))
			{
				console::warning("invalid skill type at index %lli\n", i);
				continue;
			}

			this->skill_table.emplace_back(skill);
		}

		for (auto i = 0ull; i < data["job_set_table"].size(); i++)
		{
			auto table = std::make_shared<crew_set_table_t>();
			if (!table->parse(data["job_set_table"][i], "job"))
			{
				console::warning("invalid job_set_table entry at index %lli\n", i);
				continue;
			}

			this->job_set_table[table->id] = table;
		}

		for (auto i = 0ull; i < data["skill_set_table"].size(); i++)
		{
			auto table = std::make_shared<crew_set_table_t>();
			if (!table->parse(data["skill_set_table"][i], "skill"))
			{
				console::warning("invalid skill_set_table entry at index %lli\n", i);
				continue;
			}

			this->skill_set_table[table->id] = table;
		}

		for (auto i = 0ull; i < data["type_table"].size(); i++)
		{
			auto crew_member_type = std::make_shared<crew_type_t>();
			if (!crew_member_type->parse(data["type_table"][i]))
			{
				console::warning("invalid crew type at index %lli\n", i);
				continue;
			}

			this->type_table[crew_member_type->id] = crew_member_type;
		}

		for (auto i = 0ull; i < data["unique_table"].size(); i++)
		{
			auto crew_member_type = std::make_shared<crew_unique_type_t>();
			if (!crew_member_type->parse(data["unique_table"][i]))
			{
				console::warning("invalid unique crew type at index %lli\n", i);
				continue;
			}

			this->unique_table[crew_member_type->id] = crew_member_type;
		}

		for (auto i = 0ull; i < data["sick_table"].size(); i++)
		{
			auto sickness = std::make_shared<crew_sickness_t>();
			if (!sickness->parse(data["sick_table"][i]))
			{
				console::warning("invalid sickness at index %lli\n", i);
				continue;
			}

			this->sick_table.emplace_back(sickness);
		}

		return true;
	}

	std::shared_ptr<crew_skill_t> ssd_crew_generator_table::find_skill(const std::uint32_t type, const std::uint32_t rank)
	{
		const auto iter = std::ranges::find_if(this->skill_table.begin(), this->skill_table.end(), [&](const std::shared_ptr<crew_skill_t>& skill)
		{
			return skill->id == type && skill->level == rank;
		});

		if (iter == this->skill_table.end())
		{
			return nullptr;
		}

		return *iter;
	}

	std::uint32_t ssd_crew_generator_table::roll_skill(const std::uint32_t skill_set_table_id)
	{
		const auto iter = this->skill_set_table.find(skill_set_table_id);
		if (iter == this->skill_set_table.end())
		{
			return 0u;
		}

		auto& engine = utils::random::get_engine();
		std::discrete_distribution<> dist(iter->second->ratios.begin(), iter->second->ratios.end());

		const auto idx = static_cast<std::size_t>(dist(engine));
		if (idx >= iter->second->values.size())
		{
			return 0u;
		}

		return iter->second->values[idx];
	}

	std::uint32_t ssd_crew_generator_table::roll_job(const std::uint32_t job_set_table_id)
	{
		const auto iter = this->job_set_table.find(job_set_table_id);
		if (iter == this->job_set_table.end())
		{
			return 0u;
		}

		auto& engine = utils::random::get_engine();
		std::discrete_distribution<> dist(iter->second->ratios.begin(), iter->second->ratios.end());

		const auto idx = static_cast<std::size_t>(dist(engine));
		if (idx >= iter->second->values.size())
		{
			return 0u;
		}

		return iter->second->values[idx];
	}
}
