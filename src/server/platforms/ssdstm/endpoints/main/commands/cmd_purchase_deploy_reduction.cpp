#include <std_include.hpp>

#include "cmd_purchase_deploy_reduction.hpp"

namespace emulator::ssd
{
	json::value cmd_purchase_deploy_reduction::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		std::uint32_t team_index{};
		if (!json::read(team_index, data["team_index"]))
		{
			return error(ERR_INVALIDARG);
		}

		result["purchase_result"]["is_coin"] = 0;
		result["purchase_result"]["payment"] = 0;
		result["purchase_result"]["balance"] = 0;

		auto& reward_info = result["deploy_result"]["reward_info"];
		reward_info["text_id"] = 0;
		reward_info["expire_date"] = 0;
		reward_info["present_list"] = json::array();
		reward_info["resources_list"] = json::array();
		reward_info["stackable_list"] = json::array();
		reward_info["nonstackable_list"] = json::array();
		reward_info["battle_pack_list"] = json::array();
		reward_info["recipe_list"] = json::array();

		result["result"] = "ERR_NOTIMPLEMENTED";

		return result;
	}

	std::uint32_t cmd_purchase_deploy_reduction::flags()
	{
		return CMD_NEEDS_USER;
	}
}
