#include <std_include.hpp>

#include "cmd_craft_bp_lottery.hpp"

#include "database/models/present_box.hpp"

#include "game/lottery.hpp"
#include "utils/time.hpp"

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

		const auto expire_date = utils::time::from_now(14d);

		auto& lottery_list_j = result["battle_pack_lottery_list"];

		database::users::give_item_params_t give_params{};
		give_params.stackable_list = &stackable_list;
		give_params.nonstackable_list = &nonstackable_list;
		give_params.resource_list = &inventory_resource_list;
		give_params.player_inventory = &player_inventory;
		give_params.user_inventory = &user_inventory;

		std::vector<game::item_t> presents;

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
				database::users::reward_info_t lottery_entry{};
				auto& lottery_entry_j = lottery_list_j[lottery_list_j.size()];

				const auto item = lottery->second();
				if (!user->give_item(item, give_params, &lottery_entry))
				{
					presents.emplace_back(item);
					lottery_entry.present_list.emplace_back(item);
					lottery_entry.expire_date = expire_date;
				}

				lottery_entry.to_json(lottery_entry_j, true);
				lottery_entry_j["battle_pack_id"] = bp_def->id;
			}
		}

		database::present_box::add_items(user->get_user_id(),
			database::present_box::present_flag_expire, 
			expire_date, presents);

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
