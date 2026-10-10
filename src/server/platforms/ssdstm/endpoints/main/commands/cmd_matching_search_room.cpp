#include <std_include.hpp>

#include "cmd_matching_search_room.hpp"

#include "database/models/matching.hpp"

namespace emulator::ssd
{
	json::value cmd_matching_search_room::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		database::matching::search_param_t param{};
		if (!json::read(param, data["search_param"]) || param.max > 16u)
		{
			return error(ERR_INVALIDARG);
		}

		const auto rooms = database::matching::search_rooms(user->current_player->get_player_id(), param);
		for (auto i = 0ull; i < rooms.size(); i++)
		{
			const auto owner_user = database::users::find_from_current_player(rooms[i].get_owner_player_id());
			if (!owner_user.has_value())
			{
				continue;
			}

			console::debug("search room found with params: \n");
			console::debug("\tflag_attr: %u %u\n", param.flag_attr, rooms[i].get_flag_attr());
			console::debug("\tflag_filter: %u %u\n", param.flag_filter, rooms[i].get_flag_filter());
			for (auto o = 0; o < 16; o++)
			{
				console::debug("\tint_attr[%i]: %u %u\n", o, param.int_attr_param[o], rooms[i].int_attr[o]);
			}

			const auto members = database::matching::get_members(rooms[i].get_room_id());

			auto& entry = result["room_data"][i];
			auto& owner_data = entry["owner_data"];
			const auto password = rooms[i].get_password();

			entry["room_id"] = rooms[i].get_room_id();
			entry["max_slot"] = rooms[i].get_max_slot();
			entry["region_matching_level"] = rooms[i].get_region_matching_level();
			entry["owner_player_id"] = rooms[i].get_owner_player_id();
			entry["owner_display_name"] = "";
			entry["member_num"] = members.size();
			entry["reserve_num"] = 0;
			entry["flag_attr"] = rooms[i].get_flag_attr();
			entry["enable_password"] = std::uint8_t(!password.empty());

			entry["room_searchable_int_attr_external"] = json::array();
			entry["room_searchable_bin_attr_external"] = json::array();

			owner_data["npid"]["handler"]["data"] = "";
			owner_data["npid"]["handler"]["dummy"] = json::array{0, 0, 0};
			owner_data["npid"]["handler"]["term"] = 0;
			owner_data["npid"]["opt"] = json::array{0, 0, 0, 0, 0, 0, 0, 0};
			owner_data["npid"]["reserved"] = json::array{0, 0, 0, 0, 0, 0, 0, 0};
			owner_data["xuid"] = owner_user->get_account_id();
			owner_data["steamid"] = owner_user->get_account_id();
			owner_data["secure_device_address"] = "NotImplement";
			owner_data["port"] = owner_user->get_ex_port();
			owner_data["ip"] = owner_user->get_ex_ip();
			owner_data["account_id"]["id"] = owner_user->get_account_id();
			owner_data["account_id"]["type"] = 0;
		}

		return result;
	}

	std::uint32_t cmd_matching_search_room::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
