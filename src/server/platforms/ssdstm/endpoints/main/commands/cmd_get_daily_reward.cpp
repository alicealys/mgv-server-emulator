#include <std_include.hpp>

#include "cmd_get_daily_reward.hpp"

// not implemented
namespace emulator::ssd
{
	nlohmann::json cmd_get_daily_reward::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;

		// TODO
		result["reward"]["energy"] = 0;
		result["reward"]["kub_boost_flag"] = 0;
		result["reward"]["present_box_num"] = 0;
		result["reward"]["battle_pack_list"] = nlohmann::json::array();
		result["reward"]["nonstackable_list"] = nlohmann::json::array();
		result["reward"]["present_list"] = nlohmann::json::array();
		result["reward"]["recipe_list"] = nlohmann::json::array();
		result["reward"]["resources_list"] = nlohmann::json::array();
		result["reward"]["stackable_list"] = nlohmann::json::array();
		result["reward"]["text_id"] = 0;

        return result;
	}
}
