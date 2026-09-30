#include <std_include.hpp>

#include "cmd_present_box_get_item.hpp"

#include "database/models/present_box.hpp"

namespace emulator::ssd
{
	json::value cmd_present_box_get_item::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		std::uint64_t present_id{};
		if (!json::read(present_id, data["id"]))
		{
			return error(ERR_INVALIDARG);
		}

		const auto item = database::present_box::find_item(present_id);
		if (!item.has_value())
		{
			return error(ERR_NOT_FOUND);
		}

		const auto now = std::chrono::system_clock::now();
		if (item->get_player_id() != user->current_player->get_player_id() || item->get_expire_date() < now.time_since_epoch())
		{
			return error(ERR_NOT_FOUND);
		}
		database::players::stackable_item_list_t stackable_list{};
		database::players::nonstackable_item_list_t nonstackable_list{};
		database::players::inventory_resource_list_t resource_list{};
		database::players::player_inventory_t player_inventory{};

		user->current_player->get_stackable_item_list(stackable_list);
		user->current_player->get_nonstackable_item_list(nonstackable_list);
		user->current_player->get_inventory_resource_list(resource_list);
		user->current_player->get_inventory(player_inventory);

		database::users::give_item_params_t give_params{};
		give_params.resource_list = &resource_list;
		give_params.stackable_list = &stackable_list;
		give_params.nonstackable_list = &nonstackable_list;
		give_params.player_inventory = &player_inventory;

		json::value empty;
		if (!user->give_item(item->get_reward(), give_params, empty))
		{
			return error(ERR_DATABASE);
		}

		result["present"]["id"] = item->get_present_id();
		result["present"]["param1"] = item->get_reward().param1;
		result["present"]["param2"] = item->get_reward().param2;
		result["present"]["param3"] = item->get_reward().param3;
		result["present"]["param4"] = item->get_reward().param4;
		result["present"]["param5"] = item->get_reward().param5;

		user->current_player->set_stackable_item_list(stackable_list);
		user->current_player->set_nonstackable_item_list(nonstackable_list);
		user->current_player->set_inventory_resource_list(resource_list);
		user->current_player->set_inventory(player_inventory);

		database::present_box::delete_item(item->get_present_id());

		return result;
	}

	std::uint32_t cmd_present_box_get_item::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
