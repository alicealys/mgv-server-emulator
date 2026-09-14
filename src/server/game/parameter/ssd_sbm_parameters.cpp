#include <std_include.hpp>

#include "ssd_sbm_parameters.hpp"

namespace game::parameters
{
	ssd_sbm_parameters::ssd_sbm_parameters()
	{
		this->load("SsdSbmParameters");
	}

	bool ssd_sbm_parameters::parse(glz::json& data)
	{
		for (auto i = 0ull; i < data["production"].size(); i++)
		{
			auto prod = std::make_shared<production_t>();
			if (!prod->parse(data["production"][i]))
			{
				console::warning("invalid production at index %lli\n", i);
			}

			this->productions[prod->id] = prod;
		}

		for (auto i = 0ull; i < data["recipe"].size(); i++)
		{
			auto recipe = std::make_shared<recipe_t>();
			if (!recipe->parse(data["recipe"][i]))
			{
				console::warning("invalid recipe at index %lli\n", i);
				continue;
			}

			const auto iter = this->productions.find(recipe->production_id);
			if (iter == this->productions.end())
			{
				console::warning("recipe %s has invalid production id %lli\n", recipe->id_str.data(), recipe->production_id);
			}
			else
			{
				recipe->production = iter->second;
			}

			this->recipes[recipe->id] = recipe;
		}

		for (auto i = 0ull; i < data["gradeup_spec"].size(); i++)
		{
			gradeup_spec_t spec{};
			spec.parse(data["gradeup_spec"][i]);
			this->gradeup_spec[spec.rarity] = spec;
		}

		for (auto i = 0ull; i < data["survival"].size(); i++)
		{
			auto survival_gear = std::make_shared<survival_gear_t>();
			if (!survival_gear->parse(data["survival"][i]))
			{
				console::warning("invalid survival at index %lli\n", i);
			}

			this->survival_gears[survival_gear->id] = survival_gear;
		}

		return true;
	}
}
