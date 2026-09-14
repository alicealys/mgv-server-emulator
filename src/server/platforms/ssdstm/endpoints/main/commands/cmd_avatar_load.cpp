#include <std_include.hpp>

#include "cmd_avatar_load.hpp"

namespace emulator::ssd
{
	json::value cmd_avatar_load::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		if (!user->current_player.has_value())
		{
			return error(ERR_PLAYER_NOTFOUND);
		}

		auto avatar = std::make_unique<database::players::avatar_t>();
		user->current_player->get_avatar(*avatar);
		avatar->to_json(result["avatar"]);

        return result;
	}

	std::uint32_t cmd_avatar_load::flags()
	{
		return CMD_NEEDS_USER;
	}
}
