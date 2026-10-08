#include <std_include.hpp>

#include "cmd_matching_migration_get_roommember_data.hpp"

#include "database/models/matching.hpp"

namespace emulator::ssd
{
	json::value cmd_matching_migration_get_roommember_data::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		std::uint64_t room_id{};
		if (!json::read(room_id, data["room_id"]))
		{
			return error(ERR_INVALIDARG);
		}

		const auto room = database::matching::get_room_from_member(user->current_player->get_player_id());
		if (!room.has_value())
		{
			return error(ERR_ROOM_NOT_FOUND);
		}

		if (room->get_room_id() != room_id)
		{
			return error(ERR_NOT_FOUND);
		}

		const auto members = database::matching::get_members(room->get_room_id());
		for (auto i = 0ull; i < members.size(); i++)
		{
			auto& member_data = result["roommember_data"][i];
			member_data["npid"]["handler"]["data"] = "";
			member_data["npid"]["handler"]["dummy"] = json::array{0, 0, 0};
			member_data["npid"]["handler"]["term"] = 0;
			member_data["npid"]["opt"] = json::array{0, 0, 0, 0, 0, 0, 0, 0};
			member_data["npid"]["reserved"] = json::array{0, 0, 0, 0, 0, 0, 0, 0};
			member_data["xuid"] = members[i].get_account_id();
			member_data["steamid"] = members[i].get_account_id();
			member_data["secure_device_address"] = "NotImplement";
			member_data["port"] = members[i].get_ex_port();
			member_data["ip"] = members[i].get_ex_ip();
			member_data["account_id"]["id"] = members[i].get_account_id();
			member_data["account_id"]["type"] = 0;
		}

		return result;
	}

	std::uint32_t cmd_matching_migration_get_roommember_data::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
