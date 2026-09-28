#include <std_include.hpp>

#include "cmd_crew_add.hpp"

#include "database/models/crew_members.hpp"

namespace emulator::ssd
{
	json::value cmd_crew_add::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		auto& add_crew_param = data["add_crew_param"];
		if (!add_crew_param.is_object())
		{
			return error(ERR_INVALIDARG);
		}

		database::crew_members::member_params_t new_member_params{};
		if (!new_member_params.parse_add_param(add_crew_param))
		{
			return error(ERR_INVALIDARG);
		}

		const auto added_member_id = database::crew_members::create(user->current_player->get_player_id(), new_member_params);
		if (added_member_id == 0ull)
		{
			return error(ERR_DATABASE);
		}

		const auto new_member = database::crew_members::find(user->current_player->get_player_id(), added_member_id);
		new_member->to_json(result["added_crew"]);

		return result;
	}

	std::uint32_t cmd_crew_add::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
