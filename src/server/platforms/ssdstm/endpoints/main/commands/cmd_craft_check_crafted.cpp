#include <std_include.hpp>

#include "cmd_craft_check_crafted.hpp"

namespace emulator::ssd
{
	json::value cmd_craft_check_crafted::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		std::uint16_t inventory_index{};
		if (!json::read(inventory_index, data["inventory_index"]))
		{
			return error(ERR_INVALIDARG);
		}

		auto stackable_item_list = std::make_unique<database::players::stackable_item_list_t>();
		auto nonstackable_item_list = std::make_unique<database::players::nonstackable_item_list_t>();

		user->current_player->get_stackable_item_list(*stackable_item_list);
		user->current_player->get_nonstackable_item_list(*nonstackable_item_list);

		const auto junk = nonstackable_item_list->find_at_index(inventory_index);
		if (junk == nullptr || (junk->flag & 16) == 0)
		{
			return error(ERR_NOT_FOUND);
		}

		const auto iter = game::parameters_table.ssd_sbm_parameters->recipes.find(junk->production_id);
		if (iter == game::parameters_table.ssd_sbm_parameters->recipes.end())
		{
			return error(ERR_NOT_FOUND);
		}

		auto stackable_count = 0u;
		auto nonstackable_count = 0u;

		junk->initialize(iter->second->production, iter->second->potential);
		junk->to_json(result["crafted_nonstackable_items"][nonstackable_count++]);

		if (iter->second->leftover[0].id != 0 && iter->second->leftover[0].count > 0)
		{
			const auto leftover_iter = game::parameters_table.ssd_sbm_parameters->productions.find(iter->second->leftover[0].id);
			if (leftover_iter != game::parameters_table.ssd_sbm_parameters->productions.end())
			{
				database::players::stackable_item_t stackable_item{};
				stackable_item.production_index = leftover_iter->second->index;
				stackable_item.flag = 0;
				stackable_item.count = iter->second->leftover[0].count;

				if (!stackable_item_list->add_item(stackable_item))
				{
					return error(ERR_OVER_CAPACITY);
				}

				stackable_item.count = iter->second->leftover[0].count;
				stackable_item.to_json(result["crafted_stackable_items"][stackable_count++]);
			}
		}

		user->current_player->set_stackable_item_list(*stackable_item_list);
		user->current_player->set_nonstackable_item_list(*nonstackable_item_list);

		result["left_resources"] = json::array();

		return result;
	}

	std::uint32_t cmd_craft_check_crafted::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
