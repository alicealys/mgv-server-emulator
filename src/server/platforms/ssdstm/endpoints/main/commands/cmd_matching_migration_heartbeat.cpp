#include <std_include.hpp>

#include "cmd_matching_migration_heartbeat.hpp"

#include "database/models/matching.hpp"

namespace emulator::ssd
{
	json::value cmd_matching_migration_heartbeat::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		std::uint32_t migration_sequence{};
		if (!json::read(migration_sequence, data["migration_sequence"]))
		{
			return error(ERR_INVALIDARG);
		}

		result["roommember_heartbeat_data"] = json::array();

		database::matching::set_member_migration_sequence(user->current_player->get_player_id(), migration_sequence);
		const auto room = database::matching::get_room_from_member(user->current_player->get_player_id());
		if (!room.has_value())
		{
			return result;
		}

		const auto members = database::matching::get_members(room->get_room_id());
		for (auto i = 0ull; i < members.size(); i++)
		{
			auto& member_data = result["roommember_heartbeat_data"][i];
			member_data["player_id"] = members[i].get_player_id();
			member_data["first_party_id"] = members[i].get_account_id();
			member_data["past_time"] = members[i].get_past_time();
			member_data["migration_sequence"] = members[i].get_migration_sequence();
		}

		return result;
	}

	std::uint32_t cmd_matching_migration_heartbeat::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
