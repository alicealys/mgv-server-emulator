#include <std_include.hpp>

#include "cmd_survivor_initialize.hpp"

#include "database/models/crew_members.hpp"
#include "database/models/defense_missions.hpp"
#include "database/models/deployments.hpp"

namespace emulator::ssd
{
	json::value cmd_survivor_initialize::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		std::uint64_t player_index{};
		if (!json::read(player_index, data["player_index"]))
		{
			return error(ERR_INVALIDARG);
		}

		const auto player = database::players::find_by_index(user->get_user_id(), player_index);
		if (!player.has_value())
		{
			return error(ERR_NOT_FOUND);
		}

		const auto team_count = database::deployments::get_team_count(player->get_player_id());

		database::players::initialize(player->get_player_id());
		database::defense_missions::delete_player_data(player->get_player_id());
		database::deployments::delete_player_data(player->get_player_id());
		database::crew_members::delete_player_data(player->get_player_id());

		for (auto i = 0ull; i < team_count; i++)
		{
			database::deployments::create_team(player->get_player_id());
		}

		return result;
	}

	std::uint32_t cmd_survivor_initialize::flags()
	{
		return CMD_NEEDS_USER;
	}
}
