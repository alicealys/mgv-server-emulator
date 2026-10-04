#include <std_include.hpp>

#include "cmd_present_box_get_all.hpp"

#include "database/models/present_box.hpp"

namespace emulator::ssd
{
	json::value cmd_present_box_get_all::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		const auto items = database::present_box::get_all_items(user->current_player->get_player_id());
		database::players::stackable_item_list_t stackable_list{};
		database::players::inventory_resource_list_t resource_list{};
		database::players::player_inventory_t inventory_info{};

		user->current_player->get_stackable_item_list(stackable_list);
		user->current_player->get_inventory_resource_list(resource_list);
		user->current_player->get_inventory(inventory_info);

		const auto now = std::chrono::system_clock::now();

		auto count = 0u;
		for (auto i = 0ull; i < items.size(); i++)
		{
			const auto& item = items[i];
			if (item.get_expire_date() < now.time_since_epoch())
			{
				continue;
			}

			database::users::give_item_params_t give_params{};
			give_params.resource_list = &resource_list;
			give_params.stackable_list = &stackable_list;
			give_params.player_inventory = &inventory_info;
			give_params.text_id = item.get_text_id();

			const auto& reward = item.get_reward();
			json::value empty;
			if (!user->give_item(reward, give_params, empty))
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

		user->current_player->set_stackable_item_list(stackable_list);
		user->current_player->set_inventory_resource_list(resource_list);
		user->current_player->set_inventory(inventory_info);

		database::present_box::delete_all_items(user->current_player->get_player_id());

		return result;
	}

	std::uint32_t cmd_present_box_get_all::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
