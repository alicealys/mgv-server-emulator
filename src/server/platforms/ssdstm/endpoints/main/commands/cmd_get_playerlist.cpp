#include <std_include.hpp>

#include "cmd_get_playerlist.hpp"

#include "database/models/players.hpp"

#include "utils/encoding.hpp"

namespace emulator::ssd
{
	nlohmann::json cmd_get_playerlist::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;

		auto avatar = std::make_unique<database::players::avatar_t>();
		auto loadout_list = std::make_unique<database::players::loadout_list_t>();
		auto inventory = std::make_unique<database::players::player_inventory_t>();
		auto mission_info = std::make_unique<database::players::mission_info_t>();
		auto nonstackable_list = std::make_unique<database::players::nonstackable_item_list_t>();

		const auto players = database::players::get_player_list(user->get_user_id());
		for (auto i = 0ull; i < players.size(); i++)
		{
			const auto& player = players[i];

			auto& entry = result["player_list"][i];
			auto& equipment_info = entry["equipment_info"];
			auto& status = entry["avatar_status_info"];

			player.get_avatar(*avatar);
			player.get_loadout_list(*loadout_list);
			player.get_inventory(*inventory);
			player.get_mission_info(*mission_info);
			player.get_nonstackable_item_list(*nonstackable_list);

			const auto current_loadout_idx = player.get_current_loadout();
			const auto& current_loadout = loadout_list->list[current_loadout_idx];

			avatar->to_json(entry["avatar_info"]);
			current_loadout.to_json(equipment_info["loadout"], current_loadout_idx);
			nonstackable_list->to_json(equipment_info["nonstackable_list"]);

			for (auto o = 0ull; o < ARRAYSIZE(inventory->energy_invested); o++)
			{
				equipment_info["energy_invested"][o] = inventory->energy_invested[o];
			}

			equipment_info["skill_status"] = utils::encoding::encode_base64(inventory->skill_status);

			status["equipment_slot"] = mission_info->equipment_slot;
			status["hunger"] = mission_info->hunger;
			status["injury_whole"] = mission_info->injury_whole;
			status["life"] = mission_info->life;
			status["oxygen"] = mission_info->oxygen;
			status["sequence_number"] = mission_info->sequence_number;
			status["stamina"] = mission_info->stamina;
			status["story_sequence"] = mission_info->story_sequence_number;
			status["thirst"] = mission_info->thirst;
			status["tiredness"] = mission_info->tiredness;

			for (auto o = 0; o < 8; o++)
			{
				status["injury_part"][o] = mission_info->injury_part[o];
			}

			entry["index"] = player.get_index();
			entry["name"] = player.get_name();
			entry["name_plate"] = player.get_nameplate();
			entry["playtime"] = player.get_playtime();
			entry["point"] = player.get_point();
		}

		result["player_capacity"] = 1;
		result["player_num"] = players.size();

        return result;
	}

	std::uint32_t cmd_get_playerlist::flags()
	{
		return CMD_NEEDS_USER;
	}
}
