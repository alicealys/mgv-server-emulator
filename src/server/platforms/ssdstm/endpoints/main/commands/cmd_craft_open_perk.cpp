#include <std_include.hpp>

#include "cmd_craft_open_perk.hpp"

#include "game/parameters.hpp"

namespace emulator::ssd
{
	json::value cmd_craft_open_perk::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		param_t param{};
		if (!json::read(param, data))
		{
			return error(ERR_INVALIDARG);
		}

		const auto iter = game::parameters_table.ssd_sbm_parameters->recipes.find(param.recipe_id);
		if (iter == game::parameters_table.ssd_sbm_parameters->recipes.end())
		{
			return error(ERR_NOT_FOUND);
		}

		auto resources = std::make_unique<database::players::inventory_resource_list_t>();
		auto stackable_item_list = std::make_unique<database::players::stackable_item_list_t>();
		auto nonstackable_item_list = std::make_unique<database::players::nonstackable_item_list_t>();
		auto player_inventory_info = std::make_unique<database::players::player_inventory_t>();

		user->current_player->get_inventory_resource_list(*resources);
		user->current_player->get_stackable_item_list(*stackable_item_list);
		user->current_player->get_nonstackable_item_list(*nonstackable_item_list);
		user->current_player->get_inventory(*player_inventory_info);

		// craft cost is scaled down by idk what so this is wrong, todo
		//if (!database::players::craft_recipe(*iter->second, 1, *resources, *stackable_item_list, *player_inventory_info))
		//{
		//	return error(ERR_RESOURCE_SHORTAGE);
		//}

		const auto item = nonstackable_item_list->find_at_index(param.target_item);
		if (item == nullptr)
		{
			return error(ERR_NOT_FOUND);
		}

		auto perk_idx = -1;
		for (auto i = 0ull; i < ARRAYSIZE(item->perk_list); i++)
		{
			if (item->perk_list[i].perk_id == param.perk_id)
			{
				perk_idx = static_cast<std::int32_t>(i);
				break;
			}
		}

		if (perk_idx == -1)
		{
			return error(ERR_NOT_FOUND);
		}

		if (item->perk_list[perk_idx].perk_level >= 5)
		{
			return error(ERR_OVER_CAPACITY);
		}

		item->perk_list[perk_idx].perk_level++;

		user->current_player->set_inventory_resource_list(*resources);
		user->current_player->set_stackable_item_list(*stackable_item_list);
		user->current_player->set_nonstackable_item_list(*nonstackable_item_list);
		user->current_player->set_inventory(*player_inventory_info);

		result["perk_id"] = param.perk_id;

		return result;
	}

	std::uint32_t cmd_craft_open_perk::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
