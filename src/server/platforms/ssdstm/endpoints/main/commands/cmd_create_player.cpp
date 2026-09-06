#include <std_include.hpp>

#include "cmd_create_player.hpp"

#include "database/models/players.hpp"

namespace emulator::ssd
{
	nlohmann::json cmd_create_player::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;

		const auto players = database::players::get_player_list(user->get_user_id());
		if (players.size() >= database::players::player_capacity)
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

        return result;
	}

	bool cmd_create_player::needs_user()
	{
		return true;
	}
}
