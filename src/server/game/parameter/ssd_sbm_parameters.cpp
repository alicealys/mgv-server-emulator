#include <std_include.hpp>

#include "ssd_sbm_parameters.hpp"

namespace game::parameters
{
	ssd_sbm_parameters::ssd_sbm_parameters()
	{
		this->load("SsdSbmParameters");
	}

	std::uint32_t ssd_sbm_parameters::get_resource_id(const std::uint32_t resource_index) const
	{
		if (resource_index > this->resources_list.size())
		{
			return 0u;
		}

		const auto& resource = this->resources_list[resource_index];
		if (resource == nullptr)
		{
			return 0u;
		}

		return resource->id;
	}

	std::uint32_t ssd_sbm_parameters::get_production_id(const std::uint32_t production_index) const
	{
		if (production_index > this->productions_list.size())
		{
			return 0u;
		}

		const auto& resource = this->productions_list[production_index];
		if (resource == nullptr)
		{
			return 0u;
		}

		return resource->id;
	}

	std::uint32_t ssd_sbm_parameters::get_battle_pack_id(const std::uint32_t bp_index) const
	{
		if (bp_index > this->battle_pack_list.size())
		{
			return 0u;
		}

		const auto& resource = this->battle_pack_list[bp_index];
		if (resource == nullptr)
		{
			return 0u;
		}

		return resource->id;
	}

	bool ssd_sbm_parameters::parse(json::value& data)
	{
		for (auto i = 0ull; i < data["customize_option"].size(); i++)
		{
			auto inst = std::make_shared<customize_option_t>();
			if (!inst->parse(data["customize_option"][i]))
			{
				console::warning("invalid customize_option at index %lli\n", i);
				continue;
			}

			this->customize_option[inst->id] = inst;
		}

		for (auto i = 0ull; i < data["customize_option_group"].size(); i++)
		{
			auto inst = std::make_shared<customize_option_group_t>();
			if (!inst->parse(data["customize_option_group"][i]))
			{
				console::warning("invalid customize_option_group at index %lli\n", i);
				continue;
			}

			for (auto o = 0ull; o < inst->options.size(); o++)
			{
				if (inst->options[o].optid == 0)
				{
					continue;
				}

				const auto iter = this->customize_option.find(inst->options[o].optid);
				if (iter == this->customize_option.end())
				{
					console::warning("customize_option_group %s has invalid customize_option id %lli\n", inst->id_str.data(), inst->options[o].optid);
					continue;
				}

				inst->options[o].option = iter->second;
			}

			this->customize_option_group[inst->id] = inst;
		}

		for (auto i = 0ull; i < data["customize"].size(); i++)
		{
			auto inst = std::make_shared<customize_t>();
			if (!inst->parse(data["customize"][i]))
			{
				console::warning("invalid customize at index %lli\n", i);
				continue;
			}

			for (auto o = 0ull; o < inst->option_slots.size(); o++)
			{
				if (inst->option_slot_ids[o] == 0)
				{
					continue;
				}

				const auto iter = this->customize_option_group.find(inst->option_slot_ids[o]);
				if (iter == this->customize_option_group.end())
				{
					console::warning("customize %s has invalid customize_option_group id %lli\n", inst->id_str.data(), inst->option_slot_ids[o]);
					continue;
				}

				inst->option_slots[o] = iter->second;
			}

			this->customize[inst->id] = inst;
		}


		for (auto i = 0ull; i < data["accessory"].size(); i++)
		{
			auto accessory = std::make_shared<accessory_t>();
			if (!accessory->parse(data["accessory"][i]))
			{
				console::warning("invalid accessory at index %lli\n", i);
			}

			this->accessories[accessory->id] = accessory;
		}

		for (auto i = 0ull; i < data["resource"].size(); i++)
		{
			auto res = std::make_shared<resource_t>();
			if (!res->parse(data["resource"][i]))
			{
				console::warning("invalid resource at index %lli\n", i);
				continue;
			}

			this->resources[res->id] = res;
			if (res->index >= this->resources_list.size())
			{
				this->resources_list.resize(res->index + 1ull);
			}

			this->resources_list[res->index] = res;
		}

		for (auto i = 0ull; i < data["production"].size(); i++)
		{
			auto prod = std::make_shared<production_t>();
			if (!prod->parse(data["production"][i]))
			{
				console::warning("invalid production at index %lli\n", i);
				continue;
			}

			if (prod->customize_id != 0)
			{
				const auto iter = this->customize.find(prod->customize_id);
				if (iter == this->customize.end())
				{
					console::warning("production %s has invalid customize id %lli\n", prod->id_str.data(), prod->customize_id);
				}
				else
				{
					prod->customize = iter->second;
				}
			}

			this->productions[prod->id] = prod;
			if (prod->index >= this->productions_list.size())
			{
				this->productions_list.resize(prod->index + 1ull);
			}

			this->productions_list[prod->index] = prod;
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
			if (recipe->index >= this->recipes_list.size())
			{
				this->recipes_list.resize(recipe->index + 1ull);
			}

			this->recipes_list[recipe->index] = recipe;
		}

		for (auto i = 0ull; i < data["gradeup_spec"].size(); i++)
		{
			gradeup_spec_t spec{};
			spec.parse(data["gradeup_spec"][i]);
			this->gradeup_spec[spec.rarity] = spec;
		}

		for (auto i = 0ull; i < data["battle_pack"].size(); i++)
		{
			auto res = std::make_shared<battle_pack_t>();
			if (!res->parse(data["battle_pack"][i]))
			{
				console::warning("invalid battle_pack at index %lli\n", i);
				continue;
			}

			this->battle_packs[res->id] = res;
			if (res->index >= this->battle_pack_list.size())
			{
				this->battle_pack_list.resize(res->index + 1ull);
			}

			this->battle_pack_list[res->index] = res;
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
