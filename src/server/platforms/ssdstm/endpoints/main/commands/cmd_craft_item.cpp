#include <std_include.hpp>

#include "cmd_craft_item.hpp"

#include "game/parameters.hpp"

namespace emulator::ssd
{
	json::value cmd_craft_item::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		auto& craft_item_param_j = data["craft_item_param"];
		if (!craft_item_param_j.is_object())
		{
			return error(ERR_INVALIDARG);
		}

		auto& ammo_count_j = craft_item_param_j["ammo_count"];
		auto& ammo_id_j = craft_item_param_j["ammo_id"];
		auto& craft_num_j = craft_item_param_j["craft_num"];
		auto& group_level_j = craft_item_param_j["group_level"];
		auto& group_skills_j = craft_item_param_j["group_skills"];
		auto& inventory_index_junk_j = craft_item_param_j["inventory_index_junk"];
		auto& recipe_id_j = craft_item_param_j["recipe_id"];

		if (!ammo_count_j.is_uint64() || !ammo_id_j.is_uint64() ||
			!craft_num_j.is_uint64() || !group_level_j.is_uint64() ||
			!group_skills_j.is_array() || !inventory_index_junk_j.is_uint64() ||
			!recipe_id_j.is_uint64())
		{
			return error(ERR_INVALIDARG);
		}

		const auto craft_num = craft_num_j.as<std::uint32_t>();
		const auto recipe_id = recipe_id_j.as<std::uint32_t>();

		const auto iter = game::parameters_table.ssd_sbm_parameters->recipes.find(recipe_id);
		if (iter == game::parameters_table.ssd_sbm_parameters->recipes.end())
		{
			return error(ERR_INVALIDARG);
		}

		const auto count = std::min(database::players::max_item_count, craft_num * iter->second->count);
		auto resources = std::make_unique<database::players::inventory_resource_list_t>();
		auto stackable_item_list = std::make_unique<database::players::stackable_item_list_t>();
		auto player_inventory_info = std::make_unique<database::players::player_inventory_t>();
		auto user_inventory_info = std::make_unique<database::users::user_inventory_t>();

		user->current_player->get_inventory_resource_list(*resources);
		user->current_player->get_stackable_item_list(*stackable_item_list, 1);
		user->current_player->get_inventory(*player_inventory_info);
		user->get_inventory(*user_inventory_info);

  		if (!database::players::craft_recipe(iter->second->price, iter->second->cost, craft_num,
			*resources, *stackable_item_list, *player_inventory_info))
		{
			return error(ERR_RESOURCE_SHORTAGE);
		}

		const auto survival_gear = iter->second->production->get_survival_gear();

		database::players::nonstackable_item_t nonstackable_item{};
		if (!iter->second->production->countable)
		{
			assert(survival_gear == nullptr);

			auto nonstackable_item_list = std::make_unique<database::players::nonstackable_item_list_t>();
			user->current_player->get_nonstackable_item_list(*nonstackable_item_list, 1);

			std::uint16_t free_index{};
			std::uint32_t obtain_order{};
			if (!nonstackable_item_list->find_free_index(free_index, obtain_order))
			{
				return error(ERR_OVER_CAPACITY);
			}

			nonstackable_item.life = static_cast<std::uint16_t>(iter->second->production->life);
			nonstackable_item.life_max = static_cast<std::uint16_t>(iter->second->production->life);
			nonstackable_item.production_id = iter->second->production->id;
			nonstackable_item.inventory_index = free_index;
			//nonstackable_item.obtain_order = obtain_order;
			nonstackable_item.spec = 1000;
			nonstackable_item.flag = 1;

			const auto& customize = iter->second->production->customize;
			if (customize != nullptr)
			{
				nonstackable_item.color = 255;
				nonstackable_item.color2 = 255;
				nonstackable_item.option_slot = static_cast<std::uint16_t>(customize->option_slots_min);
				auto opt_idx = 0u;
				for (auto i = 0ull; i < customize->option_slots.size(); i++)
				{
					auto& option_slot = customize->option_slots[i];
					if (option_slot == nullptr || !option_slot->not_empty)
					{
						continue;
					}

					auto& option = nonstackable_item.option_list[opt_idx++];
					for (auto o = 0ull; o < option_slot->options.size(); o++)
					{
						if (option_slot->options[o].obtained)
						{
							option.obtained |= 1 << o;
							if (option.option_id == 0)
							{
								option.option_id = option_slot->options[o].optid;
							}
						}
					}
				}
			}

			nonstackable_item_list->push(nonstackable_item);
			user->current_player->set_nonstackable_item_list(*nonstackable_item_list);
		}
		else
		{
			database::players::stackable_item_t stackable_item{};
			stackable_item.production_id = iter->second->production->id;
			stackable_item.flag = 1;
			stackable_item.count = count;

			if (!stackable_item_list->add_item(stackable_item, 0))
			{
				return error(ERR_OVER_CAPACITY);
			}

			stackable_item.count = count;
			stackable_item.to_json(result["crafted_stackable_items"][0]);

			if (survival_gear != nullptr)
			{
				player_inventory_info->set_survival_obtained(survival_gear->index, true);
			}
		}

		user_inventory_info->open_production(iter->second->production->index);

		user->current_player->set_inventory_resource_list(*resources);
		user->current_player->set_stackable_item_list(*stackable_item_list);
		user->current_player->set_inventory(*player_inventory_info);
		user->set_inventory(*user_inventory_info);

		nonstackable_item.to_json(result["crafted_nonstackable_item"]);

		result["inventory_index_junk"] = 0xFFFF;
		result["left_resources"] = json::array();

        return result;
	}

	std::uint32_t cmd_craft_item::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
