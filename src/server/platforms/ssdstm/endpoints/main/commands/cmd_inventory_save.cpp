#include <std_include.hpp>

#include "cmd_inventory_save.hpp"

#include "database/models/players.hpp"

namespace emulator::ssd
{
	nlohmann::json cmd_inventory_save::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;

		const auto player_inventory = std::make_unique<database::players::player_inventory_t>();
		const auto user_inventory = std::make_unique<database::users::user_inventory_t>();
		const auto loadout_list = std::make_unique<database::players::loadout_list_t>();
		const auto nonstackable_list = std::make_unique<database::players::nonstackable_list_t>();

		auto& player_inventory_j = data["inventory_player_info_save"];
		auto& user_inventory_j = data["inventory_user_info_save"];
		auto& loadout_list_j = data["load_out_list"];
		auto& nonstackable_list_j = data["nonstackable_add_list"];

		if (!player_inventory_j.is_object() || !user_inventory_j.is_object() || 
			!loadout_list_j.is_array() || !nonstackable_list_j.is_array())
		{
			return error(ERR_INVALIDARG);
		}

		user->get_inventory(*user_inventory);
		user->current_player->get_inventory(*player_inventory);
		user->current_player->get_loadout_list(*loadout_list);

		const auto new_loadout = std::make_unique<database::players::loadout_t>();
		const auto loadout_count = std::min(static_cast<std::size_t>(user->current_player->get_loadout_count()), loadout_list_j.size());

		std::uint16_t nameplate{};
		player_inventory->parse_save(player_inventory_j, nameplate);
		user_inventory->parse_save(user_inventory_j);

		for (auto i = 0ull; i < loadout_count; i++)
		{
			std::memset(new_loadout.get(), 0, sizeof(database::players::loadout_t));
			std::uint32_t index{};
			if (!new_loadout->parse(loadout_list_j[i], index))
			{
				return error(ERR_INVALIDARG);
			}

			if (index >= user->current_player->get_loadout_count())
			{
				continue;
			}

			std::memcpy(&loadout_list->list[index], new_loadout.get(), sizeof(database::players::loadout_t));
		}

		const auto nonstackable_count = std::min(ARRAYSIZE(nonstackable_list->list), nonstackable_list_j.size());
		for (auto i = 0ull; i < nonstackable_count; i++)
		{
			database::players::nonstackbable_t nonstackable{};
			nonstackable_list->list[i].parse(nonstackable_list_j[i], false);
		}

		user->set_inventory(*user_inventory);
		user->current_player->set_inventory(*player_inventory);
		user->current_player->set_loadout_list(*loadout_list);
		user->current_player->set_nonstackable_list(*nonstackable_list);
		user->current_player->set_nameplate(nameplate);

        return result;
	}

	std::uint32_t cmd_inventory_save::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
