#include <std_include.hpp>

#include "cmd_server_data_initialize.hpp"

namespace emulator::ssd
{
	json::value cmd_server_data_initialize::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		database::players::stackable_item_list_t stackable_list;
		database::players::nonstackable_item_list_t nonstackable_list;
		database::players::inventory_resource_list_t resource_list;
		database::players::battle_pack_list_t battle_pack_list;
		database::players::player_inventory_t player_inventory{};

		const auto loadout_list = std::make_unique<database::players::loadout_list_t>();
		const auto user_inventory = std::make_unique<database::users::user_inventory_t>();

		for (auto i = 0ull; i < ARRAYSIZE(loadout_list->list); i++)
		{
			loadout_list->list[i].initialize(static_cast<std::uint32_t>(i));
		}

		user_inventory->initialize();
		player_inventory.initialize();

		user->current_player->set_stackable_item_list(stackable_list);
		user->current_player->set_nonstackable_item_list(nonstackable_list);
		user->current_player->set_inventory_resource_list(resource_list);
		user->current_player->set_battle_pack_list(battle_pack_list);
		user->current_player->set_inventory(player_inventory);
		user->current_player->set_loadout_list(*loadout_list);
		user->set_inventory(*user_inventory);

		return result;
	}

	std::uint32_t cmd_server_data_initialize::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
