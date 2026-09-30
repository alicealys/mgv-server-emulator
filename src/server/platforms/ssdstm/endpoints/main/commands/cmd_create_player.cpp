#include <std_include.hpp>

#include "cmd_create_player.hpp"

#include "database/models/players.hpp"
#include "database/models/deployments.hpp"

namespace emulator::ssd
{
	json::value cmd_create_player::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		const auto player_count = database::players::get_player_count(user->get_user_id());
		if (player_count >= user->get_player_capacity())
		{
			return error(ERR_PLAYER_COUNT_FULL);
		}

		const auto player = database::players::create(user->get_user_id());
		if (!player.has_value())
		{
			return error(ERR_DATABASE);
		}

		result["player_id"] = player->get_player_id();
		result["index"] = player->get_index();

		if (player_count == 0)
		{
			return result;
		}

		const auto first_player = database::players::find_by_index(user->get_user_id(), 0ull);
		if (first_player.has_value())
		{
			const auto team_count = database::deployments::get_team_count(first_player->get_player_id());
			for (auto i = 0ull; i < team_count; i++)
			{
				database::deployments::create_team(first_player->get_player_id());
			}
		}

		return result;
	}

	std::uint32_t cmd_create_player::flags()
	{
		return CMD_NEEDS_USER;
	}
}
