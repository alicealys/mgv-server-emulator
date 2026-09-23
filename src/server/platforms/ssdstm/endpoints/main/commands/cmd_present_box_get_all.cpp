#include <std_include.hpp>

#include "cmd_present_box_get_all.hpp"

#include "database/models/present_box.hpp"

namespace emulator::ssd
{
	json::value cmd_present_box_get_all::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		const auto items = database::present_box::get_all_items(user->current_player->get_player_id());
		const auto stackable_list = std::make_unique<database::players::stackable_item_list_t>();
		const auto resource_list = std::make_unique<database::players::inventory_resource_list_t>();
		const auto inventory_info = std::make_unique<database::players::player_inventory_t>();

		user->current_player->get_stackable_item_list(*stackable_list);
		user->current_player->get_inventory_resource_list(*resource_list);
		user->current_player->get_inventory(*inventory_info);

		const auto now = std::chrono::system_clock::now();

		auto count = 0u;
		for (auto i = 0ull; i < items.size(); i++)
		{
			const auto& item = items[i];
			if (item.get_expire_date() < now.time_since_epoch())
			{
				continue;
			}

			const auto& reward = item.get_reward();
			if (!user->current_player->give_reward(reward, stackable_list.get(), resource_list.get(), inventory_info.get()))
			{
				continue;
			}

			auto& entry_j = result["present_list"][count++];
			entry_j["id"] = item.get_present_id();
			entry_j["param1"] = reward.param1;
			entry_j["param2"] = reward.param2;
			entry_j["param3"] = reward.param3;
			entry_j["param4"] = reward.param4;
			entry_j["param5"] = reward.param5;
		}

		user->current_player->set_stackable_item_list(*stackable_list);
		user->current_player->set_inventory_resource_list(*resource_list);
		user->current_player->set_inventory(*inventory_info);

		database::present_box::delete_all_items(user->current_player->get_player_id());

        return result;
	}

	std::uint32_t cmd_present_box_get_all::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
