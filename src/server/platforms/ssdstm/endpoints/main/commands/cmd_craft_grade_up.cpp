#include <std_include.hpp>

#include "cmd_craft_grade_up.hpp"

#include "game/parameters.hpp"

namespace emulator::ssd
{
	nlohmann::json cmd_craft_grade_up::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;

		auto& grade_up_item_j = data["grade_up_item"];
		auto& recipe_id_j = data["recipe_id"];

		if (!grade_up_item_j.is_number_unsigned() || !recipe_id_j.is_number_unsigned())
		{
			return error(ERR_INVALIDARG);
		}

		const auto grade_up_item = grade_up_item_j.get<std::uint16_t>();
		const auto recipe_id = recipe_id_j.get<std::uint32_t>();

		const auto iter = game::parameters_table.ssd_sbm_parameters->recipes.find(recipe_id);
		if (iter == game::parameters_table.ssd_sbm_parameters->recipes.end())
		{
			return error(ERR_INVALIDARG);
		}

		auto nonstackable_list = std::make_unique<database::players::nonstackable_item_list_t>();
		auto resources = std::make_unique<database::players::inventory_resource_list_t>();

		user->current_player->get_inventory_resource_list(*resources);
		user->current_player->get_nonstackable_item_list(*nonstackable_list);

		auto found = false;
		for (auto i = 0ull; i < nonstackable_list->size(); i++)
		{
			auto& item = nonstackable_list->operator[](i);
			if (item.inventory_index == grade_up_item)
			{
				if (item.production_id != iter->second->production_id && item.grade < 5)
				{
					found = false;
					break;
				}

				++item.grade;
				item.life = game::calc_gradeup_life(iter->second->production->life, item.grade);

				found = true;
				break;
			}
		}

		if (!found)
		{
			return error(ERR_DATABASE);
		}

		// correct requirements?
		// cost[i] = cost[i] * 0.15 * grade; // maybe
		//if (!database::players::craft_recipe(recipe_id, *resources))
		//{
		//	return error(ERR_DATABASE);
		//}

		user->current_player->set_inventory_resource_list(*resources);
		user->current_player->set_nonstackable_item_list(*nonstackable_list);
	
		return result;
	}

	std::uint32_t cmd_craft_grade_up::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
