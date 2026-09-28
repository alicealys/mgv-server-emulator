#include <std_include.hpp>

#include "cmd_crew_banish.hpp"

#include "database/models/crew_members.hpp"

namespace emulator::ssd
{
	json::value cmd_crew_banish::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		auto& list_j = data["unique_id_list"];
		if (!list_j.is_array())
		{
			return error(ERR_INVALIDARG);
		}

		for (auto i = 0ull; i < list_j.size(); i++)
		{
			std::uint64_t unique_id{};
			if (!json::read(unique_id, list_j[i]))
			{
				continue;
			}

			database::crew_members::remove(user->current_player->get_player_id(), unique_id);
		}

		return result;
	}

	std::uint32_t cmd_crew_banish::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
