#include <std_include.hpp>

#include "cmd_present_box_get_all.hpp"

#include "database/models/present_box.hpp"

namespace emulator::ssd
{
	json::value cmd_present_box_get_all::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		const auto items = database::present_box::get_all_items(user->get_user_id());

		database::players::stackable_item_list_t stackable_list{};
		database::players::nonstackable_item_list_t nonstackable_list{};
		database::players::inventory_resource_list_t resource_list{};
		database::players::player_inventory_t player_inventory{};
		database::players::battle_pack_list_t battle_pack_list{};
		database::users::user_inventory_t user_inventory{};

		user->current_player->get_stackable_item_list(stackable_list);
		user->current_player->get_nonstackable_item_list(nonstackable_list);
		user->current_player->get_inventory_resource_list(resource_list);
		user->current_player->get_inventory(player_inventory);
		user->current_player->get_battle_pack_list(battle_pack_list);
		user->get_inventory(user_inventory);

		const auto now = std::chrono::system_clock::now();
		std::vector<std::uint64_t> delete_present_ids;

		auto count = 0u;
		for (auto i = 0ull; i < items.size(); i++)
		{
			const auto& item = items[i];
			if (item.get_expire_date() < now.time_since_epoch())
			{
				database::present_box::delete_item(item.get_present_id());
				continue;
			}

			database::users::give_item_params_t give_params{};
			give_params.resource_list = &resource_list;
			give_params.stackable_list = &stackable_list;
			give_params.nonstackable_list = &nonstackable_list;
			give_params.player_inventory = &player_inventory;
			give_params.battle_pack_list = &battle_pack_list;
			give_params.user_inventory = &user_inventory;
			give_params.text_id = item.get_text_id();

			const auto& reward = item.get_reward();
			if (!user->give_item(reward, give_params))
			{
				continue;
			}

			item.to_json_present(result["present_list"][count++]);
			delete_present_ids.emplace_back(item.get_present_id());
		}

		user->current_player->set_stackable_item_list(stackable_list);
		user->current_player->set_nonstackable_item_list(nonstackable_list);
		user->current_player->set_inventory_resource_list(resource_list);
		user->current_player->set_inventory(player_inventory);
		user->current_player->set_battle_pack_list(battle_pack_list);
		user->set_inventory(user_inventory);
		database::present_box::delete_items(delete_present_ids);

		return result;
	}

	std::uint32_t cmd_present_box_get_all::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
