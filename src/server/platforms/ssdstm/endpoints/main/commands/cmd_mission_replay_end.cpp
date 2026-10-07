#include <std_include.hpp>

#include "cmd_mission_replay_end.hpp"

#include "database/models/present_box.hpp"

namespace emulator::ssd
{
	json::value cmd_mission_replay_end::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		param_t param{};

		auto replay_info_list = user->current_player->get_replay_info_list();
		if (!replay_info_list.parse_diff_single(data))
		{
			return error(ERR_INVALIDARG);
		}

		user->current_player->set_replay_info_list(replay_info_list);

		result["info"]["mission_code"] = param.mission_code;
		result["info"]["difficulty"] = param.difficulty;
		result["info"]["is_clear"] = param.is_clear;
		result["info"]["is_reward"] = 1;

		result["reward"]["energy"] = 0;
		result["reward"]["kub_boost_flag"] = 0;
		result["reward"]["present_box_num"] = database::present_box::get_present_count(user->get_user_id());
		result["reward"]["battle_pack_list"] = json::array();
		result["reward"]["nonstackable_list"] = json::array();
		result["reward"]["present_list"] = json::array();
		result["reward"]["recipe_list"] = json::array();
		result["reward"]["resources_list"] = json::array();
		result["reward"]["stackable_list"] = json::array();
		result["reward"]["text_id"] = 229691447841577;

		return result;
	}

	std::uint32_t cmd_mission_replay_end::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
