#include <std_include.hpp>

#include "cmd_craft_reforge.hpp"

namespace emulator::ssd
{
	json::value cmd_craft_reforge::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		param_t param{};
		if (!json::read(param, data))
		{
			return error(ERR_INVALIDARG);
		}

		auto resources = std::make_unique<database::players::inventory_resource_list_t>();
		auto stackable_item_list = std::make_unique<database::players::stackable_item_list_t>();
		auto nonstackable_item_list = std::make_unique<database::players::nonstackable_item_list_t>();
		auto player_inventory_info = std::make_unique<database::players::player_inventory_t>();

		user->current_player->get_inventory_resource_list(*resources);
		user->current_player->get_stackable_item_list(*stackable_item_list);
		user->current_player->get_nonstackable_item_list(*nonstackable_item_list);
		user->current_player->get_inventory(*player_inventory_info);

		const auto junk = nonstackable_item_list->find_at_index(param.inventory_index_junk);
		const auto target = nonstackable_item_list->find_at_index(param.inventory_index_target);

		if (junk == nullptr || target == nullptr || (junk->flag & 16) == 0)
		{
			return error(ERR_NOT_FOUND);
		}

		const auto iter = game::parameters_table.ssd_sbm_parameters->recipes.find(junk->production_id);
		if (iter == game::parameters_table.ssd_sbm_parameters->recipes.end())
		{
			return error(ERR_NOT_FOUND);
		}

		if (iter->second->production == nullptr || target->production_id != iter->second->production->id)
		{
			return error(ERR_INVALIDARG);
		}

		if (!database::players::craft_recipe(*iter->second, 1, *resources, *stackable_item_list, *player_inventory_info))
		{
			return error(ERR_RESOURCE_SHORTAGE);
		}

		std::memset(junk, 0, sizeof(database::players::nonstackable_item_t));
		target->initialize(iter->second->production, iter->second->potential);

		user->current_player->set_inventory_resource_list(*resources);
		user->current_player->set_stackable_item_list(*stackable_item_list);
		user->current_player->set_nonstackable_item_list(*nonstackable_item_list);
		user->current_player->set_inventory(*player_inventory_info);

		result["spec"] = target->spec;
		result["life_max"] = target->life_max;

		return result;
	}

	std::uint32_t cmd_craft_reforge::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
