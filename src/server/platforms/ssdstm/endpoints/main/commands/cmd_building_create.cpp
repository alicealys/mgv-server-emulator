#include <std_include.hpp>

#include "cmd_building_create.hpp"

namespace emulator::ssd
{
	json::value cmd_building_create::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		auto& cell_info = data["cell_info"];

		param_t param{};
		if (!json::read(param, data) || !cell_info.is_object())
		{
			return error(ERR_INVALIDARG);
		}

		const auto& recipe = game::parameters_table.ssd_sbm_parameters->get_production_recipe(param.production_id);
		if (recipe == nullptr || recipe->production->building == nullptr)
		{
			return error(ERR_INVALIDARG);
		}

		database::players::inventory_resource_list_t resource_list;
		database::players::stackable_item_list_t stackable_list;
		database::players::player_inventory_t inventory{};

		user->current_player->get_inventory_resource_list(resource_list);
		user->current_player->get_stackable_item_list(stackable_list);
		user->current_player->get_inventory(inventory);

		if (!database::players::craft_recipe(*recipe, param.production_num, resource_list, stackable_list, inventory))
		{
			return error(ERR_BUILDING_MATERIAL_SHORTAGE);
		}

		const auto building_info = std::make_unique<database::players::building_info_t>();
		user->current_player->get_building_info(*building_info);
		building_info->parse_create(cell_info);

		if (!user->current_player->set_building_info(*building_info))
		{
			return error(ERR_DATABASE);
		}

		user->current_player->set_inventory_resource_list(resource_list);
		user->current_player->set_stackable_item_list(stackable_list);
		user->current_player->set_inventory(inventory);

		return result;
	}

	std::uint32_t cmd_building_create::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
