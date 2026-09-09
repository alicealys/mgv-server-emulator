#include <std_include.hpp>

#include "cmd_inventory_save.hpp"

#include "database/models/players.hpp"

namespace emulator::ssd
{
	void cmd_inventory_save::do_save(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		auto& player_inventory_j = data["inventory_player_info_save"];
		auto& user_inventory_j = data["inventory_user_info_save"];
		auto& loadout_list_j = data["load_out_list"];
		auto& nonstackable_list_j = data["nonstackable_add_list"];
		auto& stackable_list_j = data["stackable_list"];
		auto& resource_list_j = data["resource_list"];

		if (player_inventory_j.is_object())
		{
			const auto player_inventory = std::make_unique<database::players::player_inventory_t>();
			user->current_player->get_inventory(*player_inventory);

			std::uint16_t nameplate{};
			player_inventory->parse_save(player_inventory_j, nameplate);

			user->current_player->set_nameplate(nameplate);
			user->current_player->set_inventory(*player_inventory);
		}

		if (user_inventory_j.is_object())
		{
			const auto user_inventory = std::make_unique<database::users::user_inventory_t>();
			user->get_inventory(*user_inventory);
			user_inventory->parse_save(user_inventory_j);
			user->set_inventory(*user_inventory);
		}

		if (loadout_list_j.is_array())
		{
			const auto new_loadout = std::make_unique<database::players::loadout_t>();
			const auto loadout_count = std::min(static_cast<std::size_t>(user->current_player->get_loadout_count()), loadout_list_j.size());

			const auto loadout_list = std::make_unique<database::players::loadout_list_t>();
			user->current_player->get_loadout_list(*loadout_list);

			for (auto i = 0ull; i < loadout_count; i++)
			{
				std::memset(new_loadout.get(), 0, sizeof(database::players::loadout_t));
				std::uint32_t index{};
				if (!new_loadout->parse(loadout_list_j[i], index))
				{
					continue;
				}

				if (index >= user->current_player->get_loadout_count())
				{
					continue;
				}

				std::memcpy(&loadout_list->list[index], new_loadout.get(), sizeof(database::players::loadout_t));
			}

			user->current_player->set_loadout_list(*loadout_list);
		}

		if (nonstackable_list_j.is_array())
		{
			const auto nonstackable_list = std::make_unique<database::players::nonstackable_item_list_t>();
			user->current_player->get_nonstackable_item_list(*nonstackable_list);
			nonstackable_list->parse_diff(nonstackable_list_j);
			user->current_player->set_nonstackable_item_list(*nonstackable_list);
		}

		if (stackable_list_j.is_array())
		{
			const auto stackable_list = std::make_unique<database::players::stackable_item_list_t>();
			user->current_player->get_stackable_item_list(*stackable_list);
			stackable_list->parse_diff(stackable_list_j);
			user->current_player->set_stackable_item_list(*stackable_list);
		}

		if (resource_list_j.is_array())
		{
			const auto resource_list = std::make_unique<database::players::inventory_resource_list_t>();
			user->current_player->get_inventory_resource_list(*resource_list);
			resource_list->parse_diff(resource_list_j);
			user->current_player->set_inventory_resource_list(*resource_list);
		}
	}

	nlohmann::json cmd_inventory_save::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;

		this->do_save(data, user);

        return result;
	}

	std::uint32_t cmd_inventory_save::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
