#include <std_include.hpp>

#include "cmd_get_friend_loadout.hpp"

namespace emulator::ssd
{
	json::value cmd_get_friend_loadout::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		param_t param{};
		if (!json::read(param, data))
		{
			return error(ERR_INVALIDARG);
		}

		const auto target_user = database::users::find_from_account(param.target.id);
		if (!user.has_value())
		{
			return error(ERR_NOT_FOUND);
		}

		const auto player = target_user->current_player;
		if (!player.has_value())
		{
			return error(ERR_NOT_FOUND);
		}

		database::players::nonstackable_item_list_t nonstackable_list;
		const auto loadout_list = std::make_unique<database::players::loadout_list_t>();
		const auto player_inventory = std::make_unique<database::players::player_inventory_t>();

		player->get_loadout_list(*loadout_list);
		player->get_nonstackable_item_list(nonstackable_list);
		player->get_inventory(*player_inventory);

		auto& equipment_info = result["equipment_info"];
		auto& current_loadout = loadout_list->list[0];

		current_loadout.to_json(equipment_info["loadout"], 0);

		auto& nonstackable_list_j = equipment_info["nonstackable_list"];
		nonstackable_list_j = json::array();

		{
			auto idx = 0u;
			for (auto o = 0ull; o < nonstackable_list.size(); o++)
			{
				auto& item = nonstackable_list[o];
				if (!current_loadout.has_item_index(item.inventory_index))
				{
					continue;
				}

				item.to_json(nonstackable_list_j[idx++]);
			}
		}

		for (auto i = 0ull; i < ARRAYSIZE(player_inventory->energy_invested); i++)
		{
			equipment_info["energy_invested"][i] = player_inventory->energy_invested[i];
		}

		equipment_info["skill_status"] = utils::encoding::encode_base64(player_inventory->skill_status);

		result["player_type"] = 2; // ?

		return result;
	}

	std::uint32_t cmd_get_friend_loadout::flags()
	{
		return CMD_NEEDS_USER;
	}
}
