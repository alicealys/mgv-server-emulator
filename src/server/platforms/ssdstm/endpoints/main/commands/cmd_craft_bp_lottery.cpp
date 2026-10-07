#include <std_include.hpp>

#include "cmd_craft_bp_lottery.hpp"

#include "database/models/present_box.hpp"

#include "game/battle_packs.hpp"

namespace emulator::ssd
{
	json::value cmd_craft_bp_lottery::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		auto& list_j = data["battle_pack_list"];
		if (!list_j.is_array())
		{
			return error(ERR_INVALIDARG);
		}

		database::players::stackable_item_list_t stackable_list;
		database::players::nonstackable_item_list_t nonstackable_list;
		database::players::inventory_resource_list_t inventory_resource_list;
		database::players::player_inventory_t player_inventory{};
		database::users::user_inventory_t user_inventory{};
		database::players::battle_pack_list_t inventory_battle_packs;

		user->current_player->get_stackable_item_list(stackable_list, list_j.size());
		user->current_player->get_nonstackable_item_list(nonstackable_list, list_j.size());
		user->current_player->get_inventory_resource_list(inventory_resource_list, list_j.size());
		user->current_player->get_inventory(player_inventory);
		user->current_player->get_battle_pack_list(inventory_battle_packs);

		user->get_inventory(user_inventory);

		const auto now = std::chrono::system_clock::now();
		const auto expire_date = now + 24h * 14;
		const auto expire_date_s = std::chrono::duration_cast<std::chrono::seconds>(expire_date.time_since_epoch());

		auto& result_list = result["battle_pack_lottery_list"];

		database::users::give_item_params_t give_params{};
		give_params.stackable_list = &stackable_list;
		give_params.nonstackable_list = &nonstackable_list;
		give_params.resource_list = &inventory_resource_list;
		give_params.player_inventory = &player_inventory;
		give_params.user_inventory = &user_inventory;

		for (auto i = 0ull; i < list_j.size(); i++)
		{
			entry_t entry{};
			if (!json::read(entry, list_j[i]) || entry.num > 128)
			{
				continue;
			}

			const auto bp = inventory_battle_packs.get_entry(entry.inventory_index, entry.inventory_type);
			if (bp == nullptr || bp->count < entry.num)
			{
				continue;
			}

			bp->count -= entry.num;
			if (bp->bp_index >= game::parameters_table.ssd_sbm_parameters->battle_pack_list.size())
			{
				continue;
			}

			const auto& bp_def = game::parameters_table.ssd_sbm_parameters->battle_pack_list[bp->bp_index];
			
			const auto lottery = game::battle_pack_lotteries.find(bp_def->id);
			if (lottery == game::battle_pack_lotteries.end())
			{
				continue;
			}

			for (auto o = 0u; o < entry.num; o++)
			{
				auto& result_entry = result_list[result_list.size()];
				auto& battle_pack_id_j = result_entry["battle_pack_id"];
				auto& present_list_j = result_entry["present_list"];
				auto& expire_date_j = result_entry["expire_date"];
				result_entry["resources_list"] = json::array();
				result_entry["stackable_list"] = json::array();
				result_entry["nonstackable_list"] = json::array();
				result_entry["energy"] = 0;

				present_list_j = json::array();
				battle_pack_id_j = 0;
				expire_date_j = 0;

				battle_pack_id_j = bp_def->id;

				const auto item = lottery->second();
				if (!user->give_item(item, give_params, result_entry))
				{
					database::present_box::add_item(user->get_user_id(),
						database::present_box::present_flag_expire, expire_date_s, item);
					item.to_json(present_list_j[present_list_j.size()]);
					expire_date_j = expire_date_s.count();
				}
			}
		}

		user->current_player->set_battle_pack_list(inventory_battle_packs);
		user->current_player->set_stackable_item_list(stackable_list);
		user->current_player->set_nonstackable_item_list(nonstackable_list);
		user->current_player->set_inventory_resource_list(inventory_resource_list);
		user->current_player->set_inventory(player_inventory);
		user->current_player->set_battle_pack_list(inventory_battle_packs);

		return result;
	}

	std::uint32_t cmd_craft_bp_lottery::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
