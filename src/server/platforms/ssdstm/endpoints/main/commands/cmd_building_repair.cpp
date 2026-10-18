#include <std_include.hpp>

#include "cmd_building_repair.hpp"

namespace emulator::ssd
{
	json::value cmd_building_repair::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		auto& current_life_obj_j = data["current_life"];
		auto& edge_type_j = data["edge_type"];
		auto& production_id_j = data["production_id"];

		if (!current_life_obj_j.is_object() ||
			!edge_type_j.is_uint64() || !production_id_j.is_uint64())
		{
			return error(ERR_INVALIDARG);
		}

		const auto edge_type = edge_type_j.as<std::uint32_t>();
		const auto production_id = production_id_j.as<std::uint16_t>();

		if (edge_type >= database::players::edge_type_count)
		{
			return error(ERR_INVALIDARG);
		}

		auto& column_j = current_life_obj_j["column"];
		auto& row_j = current_life_obj_j["row"];
		auto& current_life_j = current_life_obj_j["current_life"];

		if (!column_j.is_uint64() || !row_j.is_uint64() ||
			!current_life_j.is_uint64())
		{
			return error(ERR_INVALIDARG);
		}

		const auto col = column_j.as<std::uint32_t>();
		const auto row = row_j.as<std::uint32_t>();
		const auto current_life = current_life_j.as<std::uint16_t>();

		const auto building_info = std::make_unique<database::players::building_info_t>();

		database::players::inventory_resource_list_t resource_list;
		database::players::stackable_item_list_t stackable_list;
		database::players::player_inventory_t inventory{};

		user->current_player->get_building_info(*building_info);
		user->current_player->get_inventory_resource_list(resource_list);
		user->current_player->get_stackable_item_list(stackable_list);
		user->current_player->get_inventory(inventory);

		auto& edge = building_info->cells[row][col].edges[edge_type];
		if (!edge.is_valid(edge_type) || edge.production_index != production_id || current_life > edge.max_life)
		{
			return error(ERR_DATABASE);
		}

		const auto& recipe = game::parameters_table.ssd_sbm_parameters->get_production_recipe(edge.production_index);
		if (recipe == nullptr)
		{
			return error(ERR_DATABASE);
		}

		const auto scalar = (1.f - (static_cast<float>(current_life) / static_cast<float>(edge.max_life))) * 0.5f;
		if (!database::players::craft_recipe(*recipe, 1, resource_list, stackable_list, inventory, scalar))
		{
			return error(ERR_BUILDING_MATERIAL_SHORTAGE);
		}

		edge.life = edge.max_life;

		user->current_player->set_building_info(*building_info);
		user->current_player->set_inventory_resource_list(resource_list);
		user->current_player->set_stackable_item_list(stackable_list);
		user->current_player->set_inventory(inventory);

		return result;
	}

	std::uint32_t cmd_building_repair::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
