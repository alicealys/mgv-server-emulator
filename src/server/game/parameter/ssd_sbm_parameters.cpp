#include <std_include.hpp>

#include "component/console.hpp"

#include "ssd_sbm_parameters.hpp"

namespace game::parameters
{
	ssd_sbm_parameters::ssd_sbm_parameters()
	{
		this->load("SsdSbmParameters");
	}

	bool ssd_sbm_parameters::parse(nlohmann::json& data)
	{
		for (auto i = 0ull; i < data["production"].size(); i++)
		{
			auto prod = std::make_shared<production_t>();
			prod->parse(data["production"][i]);
			this->productions[prod->id] = prod;
		}

		for (auto i = 0ull; i < data["recipe"].size(); i++)
		{
			auto recipe = std::make_shared<recipe_t>();
			recipe->parse(data["recipe"][i]);

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

		return true;
	}
}
