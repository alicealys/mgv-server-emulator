#include <std_include.hpp>

#include "cmd_matching_get_room_data.hpp"

#include "database/models/matching.hpp"

namespace emulator::ssd
{
	json::value cmd_matching_get_room_data::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		std::uint64_t room_id{};
		if (!json::read(room_id, data["room_id"]))
		{
			return error(ERR_INVALIDARG);
		}

		const auto room = database::matching::get_room(room_id);
		if (!room.has_value())
		{
			return error(ERR_ROOM_NOT_FOUND);
		}

		auto& room_data = result["room_data"];
		auto& owner_data = room_data["owner_data"];

		room_data["room_id"] = room->get_room_id();
		room_data["max_slot"] = room->get_max_slot();

		const auto owner_user = database::users::find_from_current_player(room->get_owner_id());
		if (!owner_user.has_value())
		{
			return result;
		}

		owner_data["npid"]["handler"]["data"] = "";
		owner_data["npid"]["handler"]["dummy"] = json::array{0, 0, 0};
		owner_data["npid"]["handler"]["term"] = 0;
		owner_data["npid"]["opt"] = json::array{0, 0, 0, 0, 0, 0, 0, 0};
		owner_data["npid"]["reserved"] = json::array{0, 0, 0, 0, 0, 0, 0, 0};
		owner_data["xuid"] = owner_user->get_account_id();;
		owner_data["steamid"] = owner_user->get_account_id();
		owner_data["secure_device_address"] = "NotImplement";
		owner_data["port"] = owner_user->get_ex_port();
		owner_data["ip"] = owner_user->get_ex_ip();
		owner_data["account_id"]["id"] = owner_user->get_account_id();
		owner_data["account_id"]["type"] = 0;

		const auto members = database::matching::get_members(room->get_room_id());

		room_data["owner_player_id"] = room->get_owner_id();
		room_data["owner_display_name"] = "";
		room_data["member_num"] = members.size();
		room_data["reserve_num"] = 0;
		room_data["flag_attr"] = room->get_flag_attr();
		room_data["enable_password"] = std::uint8_t(!room->get_password().empty());
		room_data["room_searchable_int_attr_external"] = room->int_attr;
		room_data["room_searchable_bin_attr_external"][0]["data"] = room->bin_attr[0];
		room_data["room_searchable_bin_attr_external"][1]["data"] = room->bin_attr[1];
		room_data["coop_mission_id"] = room->get_mission_id();
		room_data["status"] = room->get_status();

		return result;
	}

	std::uint32_t cmd_matching_get_room_data::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
