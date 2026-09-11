#include <std_include.hpp>

#include "cmd_get_friend_headers.hpp"

#include "database/models/players.hpp"

namespace emulator::ssd
{
	nlohmann::json cmd_get_friend_headers::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;

		result["header_list"] = nlohmann::json::array();

		auto& account_list_j = data["account_list"];
		if (!account_list_j.is_array())
		{
			return result;
		}

		auto idx = 0;
		for (auto i = 0ull; i < account_list_j.size(); i++)
		{
			auto& entry = account_list_j[i];
			if (!entry.is_object())
			{
				continue;
			}

			auto& id_j = entry["id"];
			auto& type_j = entry["type"];

			if (!id_j.is_number_unsigned() || !type_j.is_number_unsigned())
			{
				continue;
			}

			const auto id = id_j.get<std::uint64_t>();
			const auto type = type_j.get<std::uint32_t>();

			if (type != 3u)
			{
				continue;
			}

			const auto friend_user = database::users::find_from_account(id);
			if (!friend_user.has_value() || !friend_user->current_player.has_value())
			{
				continue;
			}

			const auto player_inventory_info = std::make_unique<database::players::player_inventory_t>();
			friend_user->current_player->get_inventory(*player_inventory_info);

			auto& result_entry = result["header_list"][idx++];
			result_entry["account_id"]["id"] = id;
			result_entry["account_id"]["type"] = type;
			result_entry["current_class"] = player_inventory_info->class_opened; // idk
			result_entry["energy_invested_base"] = player_inventory_info->energy_invested[0]; // idk
			result_entry["energy_invested_class"] = player_inventory_info->energy_invested[0]; // idk
			result_entry["name_plate"] = friend_user->current_player->get_nameplate();
		}

        return result;
	}
}
