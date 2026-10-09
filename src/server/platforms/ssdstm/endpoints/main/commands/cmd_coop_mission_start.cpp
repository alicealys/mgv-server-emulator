#include <std_include.hpp>

#include "cmd_coop_mission_start.hpp"
#include "cmd_inventory_save.hpp"

#include "database/models/matching.hpp"

namespace emulator::ssd
{
	json::value cmd_coop_mission_start::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		param_t param{};
		if (!json::read(param, data))
		{
			return error(ERR_INVALIDARG);
		}

		const auto iter = game::parameters_table.ssd_ui_coop_mission_info->coop_missions.find(param.mission_code);
		if (iter == game::parameters_table.ssd_ui_coop_mission_info->coop_missions.end())
		{
			return error(ERR_NOT_FOUND);
		}

		const auto current_room = database::matching::get_room_from_member(user->current_player->get_player_id());
		if (current_room.has_value() && current_room->get_owner_id() == user->current_player->get_player_id())
		{
			database::matching::set_room_mission_id(current_room->get_room_id(), param.mission_code);
			database::matching::set_room_status(current_room->get_room_id(), database::matching::status_mission_start);
		}

		result["score_threshold"]["mission_id"] = param.mission_code;

		for (auto i = 0u; i < 6; i++)
		{
			if (i < iter->second->rank_threshold.size())
			{
				result["score_threshold"]["score_threshold"][i]["lottery"] = 1;
				result["score_threshold"]["score_threshold"][i]["rank"] = i;
				result["score_threshold"]["score_threshold"][i]["threshold"] = iter->second->rank_threshold[i];
			}
			else
			{
				result["score_threshold"]["score_threshold"][i]["lottery"] = 0;
				result["score_threshold"]["score_threshold"][i]["rank"] = i;
				result["score_threshold"]["score_threshold"][i]["threshold"] = 0;
			}
		}

		return result;
	}

	std::uint32_t cmd_coop_mission_start::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
