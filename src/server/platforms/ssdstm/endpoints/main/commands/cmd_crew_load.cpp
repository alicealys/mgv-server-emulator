#include <std_include.hpp>

#include "cmd_crew_load.hpp"

namespace emulator::ssd
{
	json::value cmd_crew_load::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		auto crew_levels = std::make_unique<database::players::crew_levels_t>();
		auto crew_member_list = std::make_unique<database::players::crew_member_list_t>();

		user->current_player->get_crew_levels(*crew_levels);
		user->current_player->get_crew_member_list(*crew_member_list);

		crew_member_list->to_json(result["crew_list"]);
		crew_levels->to_json(result["group_level"]);

        return result;
	}

	std::uint32_t cmd_crew_load::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
