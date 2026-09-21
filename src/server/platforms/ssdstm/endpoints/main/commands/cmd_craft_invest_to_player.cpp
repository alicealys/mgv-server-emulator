#include <std_include.hpp>

#include "cmd_craft_invest_to_player.hpp"

#include <utils/json_utils.hpp>

namespace emulator::ssd
{
	json::value cmd_craft_invest_to_player::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		param_t param{};
		if (!json::read(param, data))
		{
			return error(ERR_INVALIDARG);
		}

		std::uint8_t survival_slot_new[2]{};
		if (!utils::json_utils::parse_base64(data["survival_slot_new"], survival_slot_new))
		{
			return error(ERR_INVALIDARG);
		}

		const auto player_inventory_info = std::make_unique<database::players::player_inventory_t>();
		const auto player_play_record = std::make_unique<database::players::player_play_record_t>();
		if (param.cls >= ARRAYSIZE(player_inventory_info->energy_invested))
		{
			return error(ERR_INVALIDARG);
		}

		user->current_player->get_inventory(*player_inventory_info);
		user->current_player->get_play_record(*player_play_record);
		if (param.energy > player_inventory_info->energy)
		{
			return error(ERR_DATABASE);
		}

		player_inventory_info->energy -= param.energy;
		player_inventory_info->energy_invested[param.cls] += param.energy;
		std::memcpy(player_inventory_info->survival_new, survival_slot_new, sizeof(player_inventory_info->survival_new));
		player_play_record->first[187] += param.energy;

		user->current_player->set_inventory(*player_inventory_info);
		user->current_player->set_play_record(*player_play_record);

		result["energy"] = param.energy;

        return result;
	}

	std::uint32_t cmd_craft_invest_to_player::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
