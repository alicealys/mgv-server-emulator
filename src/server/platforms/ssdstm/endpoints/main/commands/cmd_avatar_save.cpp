#include <std_include.hpp>

#include "cmd_avatar_save.hpp"

#include "database/models/players.hpp"

namespace emulator::ssd
{
	glz::json cmd_avatar_save::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;

		database::players::avatar_t avatar{};

		auto& avatar_j = data["avatar"];
		if (!avatar_j.is_object() || !avatar.parse(avatar_j))
		{
			return error(ERR_INVALIDARG);
		}

		user->current_player->set_avatar(avatar);

        return result;
	}

	std::uint32_t cmd_avatar_save::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
