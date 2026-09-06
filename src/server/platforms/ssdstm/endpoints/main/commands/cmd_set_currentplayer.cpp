#include <std_include.hpp>

#include "cmd_set_currentplayer.hpp"

#include "database/models/players.hpp"

namespace emulator::ssd
{
	nlohmann::json cmd_set_currentplayer::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;

		auto& index_j = data["index"];
		if (!index_j.is_number_unsigned())
		{
			return error(ERR_INVALIDARG);
		}

		const auto index = index_j.get<std::uint64_t>();
		const auto player = database::players::find_by_index(user->get_user_id(), index);

		if (!player.has_value())
		{
			return error(ERR_PLAYER_NOTFOUND);
		}

		if (user->get_current_player_id() != player->get_player_id() && 
			!database::users::set_current_player(user->get_user_id(), player->get_player_id()))
		{
			return error(ERR_DATABASE);
		}

		result["player_id"] = player->get_player_id();

        return result;
	}

	bool cmd_set_currentplayer::needs_user()
	{
		return true;
	}
}
