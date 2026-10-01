#include <std_include.hpp>

#include "cmd_crew_load.hpp"

#include "database/models/crew_members.hpp"

namespace emulator::ssd
{
	json::value cmd_crew_load::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		auto crew_levels = user->current_player->get_crew_levels();
		crew_levels.to_json(result["group_level"]);

		result["crew_list"] = json::array();

		const auto crew_members = database::crew_members::get_all(user->current_player->get_player_id());
		for (auto i = 0ull; i < crew_members.size(); i++)
		{
			crew_members[i].to_json(result["crew_list"][i]);
		}

		return result;
	}

	std::uint32_t cmd_crew_load::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
