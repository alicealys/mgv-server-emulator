#include <std_include.hpp>

#include "cmd_base_resource_provide.hpp"

namespace emulator::ssd
{
	json::value cmd_base_resource_provide::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		auto& dirty_water_j = data["dirty_water"];
		auto& clean_water_j = data["clean_water"];
		auto& food_j = data["food"];
		auto& medical_supplies_j = data["food"];

		database::players::stackable_item_list_t stackable_list;
		const auto base_resources = std::make_unique<database::players::base_resources_t>();

		user->current_player->get_base_resources(*base_resources);
		user->current_player->get_stackable_item_list(stackable_list);

		const auto do_list = [&]<typename T>(json::value& list, std::uint32_t& out, T game::production_t::*base)
		{
			if (!list.is_array())
			{
				return NOERR;
			}

			for (auto i = 0ull; i < list.size(); i++)
			{
				entry_t entry{};
				if (!json::read(entry, list[i]) || entry.num > database::players::max_item_count)
				{
					return ERR_INVALIDARG;
				}

				const auto item = stackable_list.get_entry(entry.inventory_index, entry.inventory_type);
				if (item == nullptr)
				{
					return ERR_NOT_FOUND;
				}

				if (entry.num > item->count)
				{
					return ERR_RESOURCE_SHORTAGE;
				}

				item->count -= entry.num;

				const auto production = game::parameters_table.ssd_sbm_parameters->productions.find(item->get_production_id());
				if (production == game::parameters_table.ssd_sbm_parameters->productions.end())
				{
					return ERR_DATABASE;
				}

				const auto add = entry.num * (*production->second).*base;
				const auto capped_add = std::min(std::numeric_limits<std::uint32_t>::max() - out, add);

				out += capped_add;
			}

			return NOERR;
		};

#define DO_LIST(...) \
		{ \
			const auto res = do_list(__VA_ARGS__); \
			if (res != NOERR) \
			{ \
				return error(res); \
			} \
		} \

		DO_LIST(dirty_water_j, base_resources->params.dirty_water, &game::production_t::base_c_water);
		DO_LIST(clean_water_j, base_resources->params.clean_water, &game::production_t::base_c_water);
		DO_LIST(food_j, base_resources->params.food, &game::production_t::base_food);
		DO_LIST(medical_supplies_j, base_resources->params.medical_supplies, &game::production_t::base_medicine);

		user->current_player->set_stackable_item_list(stackable_list);
		user->current_player->set_base_resources(*base_resources);

		return result;
	}

	std::uint32_t cmd_base_resource_provide::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
