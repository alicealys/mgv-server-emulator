#include <std_include.hpp>

#include "cmd_checkpoint_load.hpp"

namespace emulator::ssd
{
	json::value cmd_checkpoint_load::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		result["story_unlock_info"] = json::null();
		result["current_mission_info"] = json::null();
		result["inventory_user_info"] = json::null();
		result["inventory_player_info"] = json::null();
		result["load_out_list"] = json::array();
		result["craft_board"] = json::array();
		result["recipe_list"] = json::array();
		result["resource_list"] = json::array();
		result["stackable_list"] = json::array();
		result["nonstackable_list"] = json::array();
		result["gimmick_save_info_afghan"] = json::null();
		result["gimmick_save_info_africa"] = json::null();
		result["gimmick_timer_info"] = json::null();

        return result;
	}

	std::uint32_t cmd_checkpoint_load::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
