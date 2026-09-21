#include <std_include.hpp>

#include "cmd_craft_obtain_option.hpp"

#include "game/parameters.hpp"

namespace emulator::ssd
{
	json::value cmd_craft_obtain_option::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		param_t param{};
		if (!json::read(param, data["obtain_option_item_param"]))
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

		const auto item = nonstackable_item_list->find_at_index(param.inventory_index);
		if (item == nullptr)
		{
			return error(ERR_DATABASE);
		}

		if (param.option_slot >= ARRAYSIZE(item->option_list))
		{
			return error(ERR_INVALIDARG);
		}

		const auto iter = game::parameters_table.ssd_sbm_parameters->productions.find(item->production_id);
		if (iter == game::parameters_table.ssd_sbm_parameters->productions.end())
		{
			return error(ERR_INVALIDARG);
		}

		const auto& customize = iter->second->customize;
		if (customize == nullptr)
		{
			return error(ERR_INVALIDARG);
		}

		const auto& option_group = customize->option_slots[param.option_slot];
		if (option_group == nullptr)
		{
			return error(ERR_INVALIDARG);
		}

		game::customize_option_t* option = nullptr;
		auto option_index = -1;
		for (auto i = 0ull; i < option_group->options.size(); i++)
		{
			if (option_group->options[i].optid == param.option_id)
			{
				option_index = static_cast<int>(i);
				option = option_group->options[i].option.get();
				break;
			}
		}

		if (option == nullptr)
		{
			return error(ERR_INVALIDARG);
		}

		if (!database::players::craft_recipe(option->price, option->cost, 1,
			*resources, *stackable_item_list, *player_inventory_info))
		{
			return error(ERR_DATABASE);
		}

		item->option_list[param.option_slot].obtained |= 1 << option_index;

		user->current_player->set_nonstackable_item_list(*nonstackable_item_list);
		user->current_player->set_stackable_item_list(*stackable_item_list);
		user->current_player->set_inventory_resource_list(*resources);
		user->current_player->set_inventory(*player_inventory_info);

		return result;
	}

	std::uint32_t cmd_craft_obtain_option::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
