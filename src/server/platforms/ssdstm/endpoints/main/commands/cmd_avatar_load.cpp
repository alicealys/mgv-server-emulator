#include <std_include.hpp>

#include "cmd_avatar_load.hpp"

namespace emulator::ssd
{
	nlohmann::json cmd_avatar_load::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;

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
