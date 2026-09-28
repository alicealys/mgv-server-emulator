#include <std_include.hpp>

#include "cmd_deploy_save_team.hpp"

#include "database/models/deployments.hpp"

namespace emulator::ssd
{
	json::value cmd_deploy_save_team::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		auto& team_list = data["team_list"];
		if (!team_list.is_array())
		{
			return error(ERR_INVALIDARG);
		}

		for (auto i = 0ull; i < team_list.size(); i++)
		{
			entry_t entry{};
			if (!json::read(entry, team_list[i]))
			{
				return error(ERR_INVALIDARG);
			}

			const auto name = utils::cryptography::base64::decode(entry.name);
			const auto team = database::deployments::find_team_by_index(user->current_player->get_player_id(), entry.index);
			if (!team.has_value())
			{
				return error(ERR_DATABASE);
			}

			team->update_crew_ids(entry.crew_id_list[0], entry.crew_id_list[1], entry.crew_id_list[2], entry.crew_id_list[3]);
			team->update_info(entry.combat, name, entry.survive);
		}

		return result;
	}

	std::uint32_t cmd_deploy_save_team::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
