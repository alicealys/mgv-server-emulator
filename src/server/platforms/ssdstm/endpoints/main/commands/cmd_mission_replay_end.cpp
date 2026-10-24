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

		database::users::reward_info_t reward_info{};
		reward_info.present_box_num = database::present_box::get_present_count(user->get_user_id());
		reward_info.text_id = game::lang_id_mission_completion_reward;
		reward_info.to_json(result["reward"]);

		return result;
	}

	std::uint32_t cmd_mission_replay_end::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
