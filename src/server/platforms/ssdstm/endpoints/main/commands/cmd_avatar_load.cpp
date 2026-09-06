#include <std_include.hpp>

#include "cmd_avatar_load.hpp"

namespace emulator::ssd
{
	nlohmann::json cmd_avatar_load::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;

		const auto& player = user->get_current_player();
		if (!player.has_value())
		{
			return error(ERR_PLAYER_NOTFOUND);
		}

		auto avatar = std::make_unique<database::players::avatar_t>();
		player->get_avatar(*avatar);
		avatar->to_json(result["avatar"]);

        return result;
	}

	bool cmd_avatar_load::needs_user()
	{
		return true;
	}
}
