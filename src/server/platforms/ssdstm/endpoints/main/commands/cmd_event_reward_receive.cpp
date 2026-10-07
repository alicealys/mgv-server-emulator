#include <std_include.hpp>

#include "cmd_event_reward_receive.hpp"

// not implemented
namespace emulator::ssd
{
	json::value cmd_event_reward_receive::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		std::uint32_t type{};
		if (!json::read(type, data["type"]))
		{
			return error(ERR_INVALIDARG);
		}

		// TODO
		auto& reward_info = result["reward_info"];
		reward_info["energy"] = 0;
		reward_info["kub_boost_flag"] = 0;
		reward_info["present_box_num"] = 0;
		reward_info["battle_pack_list"] = json::array();
		reward_info["nonstackable_list"] = json::array();
		reward_info["present_list"] = json::array();
		reward_info["recipe_list"] = json::array();
		reward_info["resources_list"] = json::array();
		reward_info["stackable_list"] = json::array();
		reward_info["text_id"] = 0;

		return result;
	}

	std::uint32_t cmd_event_reward_receive::flags()
	{
		return CMD_NEEDS_USER;
	}
}
