#include <std_include.hpp>

#include "cmd_mission_replay_end.hpp"

namespace emulator::ssd
{
	json::value cmd_mission_replay_end::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		param_t param{};
		if (!json::read(param, data))
		{
			return error(ERR_INVALIDARG);
		}

		result["info"]["mission_code"] = 0;
		result["info"]["difficulty"] = 0;
		result["info"]["is_clear"] = 0;
		result["info"]["is_reward"] = 0;

		result["reward"]["energy"] = 0;
		result["reward"]["kub_boost_flag"] = 0;
		result["reward"]["present_box_num"] = 0;
		result["reward"]["battle_pack_list"] = json::array();
		result["reward"]["nonstackable_list"] = json::array();
		result["reward"]["present_list"] = json::array();
		result["reward"]["recipe_list"] = json::array();
		result["reward"]["resources_list"] = json::array();
		result["reward"]["stackable_list"] = json::array();
		result["reward"]["text_id"] = 0;
        result["result"] = "ERR_NOTIMPLEMENTED";

        return result;
	}

	std::uint32_t cmd_mission_replay_end::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
