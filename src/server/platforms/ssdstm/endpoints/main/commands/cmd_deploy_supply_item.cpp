#include <std_include.hpp>

#include "cmd_deploy_supply_item.hpp"

#include "database/models/deployments.hpp"

namespace emulator::ssd
{
	json::value cmd_deploy_supply_item::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		auto& supply_list = data["supply_list"];
		if (!supply_list.is_array())
		{
			return error(ERR_INVALIDARG);
		}

		std::uint64_t team_index{};
		if (!json::read(team_index, data["team_index"]))
		{
			return error(ERR_INVALIDARG);
		}

		const auto team = database::deployments::find_team_by_index(user->current_player->get_player_id(), team_index);
		if (!team.has_value())
		{
			return error(ERR_DATABASE);
		}

		database::players::inventory_resource_list_t resource_list;
		database::players::stackable_item_list_t stackable_list;
		database::players::nonstackable_item_list_t nonstackable_list;

		user->current_player->get_inventory_resource_list(resource_list);
		user->current_player->get_stackable_item_list(stackable_list);
		user->current_player->get_nonstackable_item_list(nonstackable_list);

		struct item_update_t
		{
			database::deployments::team_item_t data;
			bool valid;
		};
		
		std::array<item_update_t, 5> items_to_update{};

		for (auto i = 0ull; i < supply_list.size(); i++)
		{
			entry_t entry{};
			if (!json::read(entry, supply_list[i]) || entry.index >= items_to_update.size())
			{
				return error(ERR_INVALIDARG);
			}

			auto& item_to_update = items_to_update[entry.index];
			if (item_to_update.valid)
			{
				continue;
			}

			if (entry.is_delete)
			{
				item_to_update.valid = true;
				continue;
			}

			if (entry.storage.count == 0)
			{
				continue;
			}

			auto current_item = team->get_item(entry.index);

			switch (entry.storage.type)
			{
			case database::deployments::item_type_resource:
			{
				const auto resource = resource_list.get_entry(entry.storage.index, database::players::inventory_storage);
				if (resource == nullptr || resource->count < entry.storage.count)
				{
					return error(ERR_SHORTAGE);
				}

				resource->count -= entry.storage.count;
				item_to_update.data.f.id_index = resource->resource_index;
				item_to_update.data.f.param1 = entry.storage.count;
				item_to_update.data.f.param2 = 0; // ?
				item_to_update.data.f.type = database::deployments::item_type_resource;
				item_to_update.valid = true;

				if (current_item.f.id_index == item_to_update.data.f.id_index &&
					current_item.f.type == item_to_update.data.f.type)
				{
					item_to_update.data.f.param1 += current_item.f.param1;
				}

				if (item_to_update.data.f.param1 > 40)
				{
					return error(ERR_OVER_CAPACITY);
				}

				break;
			}
			case database::deployments::item_type_stackable:
			{
				const auto item = stackable_list.get_entry(entry.storage.index, database::players::inventory_storage);
				if (item == nullptr || item->count < entry.storage.count)
				{
					return error(ERR_SHORTAGE);
				}

				item->count -= entry.storage.count;
				item_to_update.data.f.id_index = item->production_index;
				item_to_update.data.f.param1 = entry.storage.count;
				item_to_update.data.f.param2 = 0; // ?
				item_to_update.data.f.type = database::deployments::item_type_stackable;
				item_to_update.valid = true;

				if (current_item.f.id_index == item_to_update.data.f.id_index &&
					current_item.f.type == item_to_update.data.f.type)
				{
					item_to_update.data.f.param1 += current_item.f.param1;
				}

				if (item_to_update.data.f.param1 > 40)
				{
					return error(ERR_OVER_CAPACITY);
				}

				break;
			}
			case database::deployments::item_type_nonstackable:
			{
				const auto item = nonstackable_list.find_at_index(entry.storage.index);
				if (item == nullptr || entry.storage.count > 1)
				{
					return error(ERR_SHORTAGE);
				}

				const auto production = game::parameters_table.ssd_sbm_parameters->productions.find(item->production_id);
				if (production == game::parameters_table.ssd_sbm_parameters->productions.end())
				{
					return error(ERR_DATABASE);
				}

				item_to_update.data.f.id_index = static_cast<std::uint16_t>(production->second->index);
				item_to_update.data.f.param1 = item->life;
				item_to_update.data.f.param2 = item->life_max;
				item_to_update.data.f.type = database::deployments::item_type_nonstackable;
				item_to_update.valid = true;
				std::memset(item, 0, sizeof(database::players::nonstackable_item_t));
				break;
			}
			}
		}

		for (auto i = 0u; i < static_cast<std::uint32_t>(items_to_update.size()); i++)
		{
			if (!items_to_update[i].valid)
			{
				team->get_item(i).to_json(result["item_list"][i]);
			}
			else
			{
				team->update_item(i, items_to_update[i].data);
				items_to_update[i].data.to_json(result["item_list"][i]);
			}
		}

		user->current_player->set_inventory_resource_list(resource_list);
		user->current_player->set_stackable_item_list(stackable_list);
		user->current_player->set_nonstackable_item_list(nonstackable_list);

		return result;
	}

	std::uint32_t cmd_deploy_supply_item::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
