#include <std_include.hpp>

#include "cmd_matching_get_matching_player_num.hpp"

#include "database/models/matching.hpp"

namespace emulator::ssd
{
	json::value cmd_matching_get_matching_player_num::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;
		
		result["player_num"] = database::matching::get_total_member_count();
		result["room_num"] = database::matching::get_total_room_count();
		result["threshold1"] = 10;
		result["threshold2"] = 100;
		result["threshold3"] = 1000;

		return result;
	}

	std::uint32_t cmd_matching_get_matching_player_num::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
