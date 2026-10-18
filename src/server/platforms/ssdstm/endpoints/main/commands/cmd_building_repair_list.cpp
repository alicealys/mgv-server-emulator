#include <std_include.hpp>

#include "cmd_building_repair_list.hpp"

namespace emulator::ssd
{
	json::value cmd_building_repair_list::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		const auto building_info = std::make_unique<database::players::building_info_t>();

		database::players::inventory_resource_list_t resource_list;
		database::players::stackable_item_list_t stackable_list;
		database::players::player_inventory_t inventory{};

		user->current_player->get_building_info(*building_info);
		user->current_player->get_inventory_resource_list(resource_list);
		user->current_player->get_stackable_item_list(stackable_list);
		user->current_player->get_inventory(inventory);

		const auto do_list = [&](json::value& data, const std::uint32_t type)
		{
			if (!data.is_array())
			{
				return true;
			}

			for (auto i = 0ull; i < data.size(); i++)
			{
				auto& column_j = data[i]["column"];
				auto& row_j = data[i]["row"];
				auto& current_life_j = data[i]["current_life"];

				if (!column_j.is_uint64() || !row_j.is_uint64() || !current_life_j.is_uint64())
				{
					return false;
				}

				const auto col = column_j.as<std::uint32_t>();
				const auto row = row_j.as<std::uint32_t>();
				const auto current_life = current_life_j.as<std::uint16_t>();

				auto& edge = building_info->cells[row][col].edges[type];
				if (!edge.is_valid(type) || current_life > edge.max_life)
				{
					return false;
				}

				const auto& recipe = game::parameters_table.ssd_sbm_parameters->get_production_recipe(edge.production_index);
				if (recipe == nullptr)
				{
					return false;
				}

				const auto scalar = (1.f - (static_cast<float>(current_life) / static_cast<float>(edge.max_life))) * 0.5f;
				if (!database::players::craft_recipe(*recipe, 1, resource_list, stackable_list, inventory, scalar))
				{
					return false;
				}

				edge.life = edge.max_life;
			}

			return true;
		};

		if (!do_list(data["center_life_list"], database::players::edge_type_center) ||
			!do_list(data["upper_life_list"], database::players::edge_type_upper) ||
			!do_list(data["left_life_list"], database::players::edge_type_left))
		{
			return error(ERR_DATABASE);
		}

		user->current_player->set_building_info(*building_info);
		user->current_player->set_inventory_resource_list(resource_list);
		user->current_player->set_stackable_item_list(stackable_list);
		user->current_player->set_inventory(inventory);

		return result;
	}

	std::uint32_t cmd_building_repair_list::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
