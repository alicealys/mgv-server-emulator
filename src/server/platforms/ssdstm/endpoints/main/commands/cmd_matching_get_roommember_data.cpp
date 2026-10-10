#include <std_include.hpp>

#include "cmd_matching_get_roommember_data.hpp"

#include "database/models/matching.hpp"

namespace emulator::ssd
{
	json::value cmd_matching_get_roommember_data::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		std::uint64_t room_id{};
		if (!json::read(room_id, data["room_id"]))
		{
			return error(ERR_INVALIDARG);
		}

		const auto room = database::matching::get_room_from_member(user->current_player->get_player_id());
		if (!room.has_value() || room->get_room_id() != room_id)
		{
			return error(ERR_NOT_FOUND);
		}

		const auto members = database::matching::get_members(room->get_room_id());
		for (auto i = 0ull; i < members.size(); i++)
		{
			auto& member_data = result["roommember_data"][i];
			auto& account_data = member_data["account_data"];

			account_data["npid"]["handler"]["data"] = "";
			account_data["npid"]["handler"]["dummy"] = json::array{0, 0, 0};
			account_data["npid"]["handler"]["term"] = 0;
			account_data["npid"]["opt"] = json::array{0, 0, 0, 0, 0, 0, 0, 0};
			account_data["npid"]["reserved"] = json::array{0, 0, 0, 0, 0, 0, 0, 0};
			account_data["xuid"] = members[i].get_account_id();
			account_data["steamid"] = members[i].get_account_id();
			account_data["secure_device_address"] = "NotImplement";
			account_data["port"] = members[i].get_ex_port();
			account_data["ip"] = members[i].get_ex_ip();
			account_data["account_id"]["id"] = members[i].get_account_id();
			account_data["account_id"]["type"] = 0;

			member_data["player_id"] = members[i].get_player_id();
			member_data["first_party_id"] = members[i].get_account_id();
			member_data["display_name"] = std::format("{}_player01", members[i].get_account_id());
			member_data["role"] = std::uint8_t(members[i].get_player_id() == room->get_owner_player_id());
			member_data["past_time"] = members[i].get_past_time();
		}

		return result;
	}

	std::uint32_t cmd_matching_get_roommember_data::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
