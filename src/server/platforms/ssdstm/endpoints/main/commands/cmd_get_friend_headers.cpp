#include <std_include.hpp>

#include "cmd_get_friend_headers.hpp"

#include "database/models/players.hpp"

namespace emulator::ssd
{
	json::value cmd_get_friend_headers::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		result["header_list"] = json::array();

		auto& account_list_j = data["account_list"];
		if (!account_list_j.is_array())
		{
			return result;
		}

		auto idx = 0;
		for (auto i = 0ull; i < account_list_j.size(); i++)
		{
			auto& entry = account_list_j[i];
			if (!entry.is_object())
			{
				continue;
			}

			auto& id_j = entry["id"];
			auto& type_j = entry["type"];

			if (!id_j.is_uint64() || !type_j.is_uint64())
			{
				continue;
			}

			const auto id = id_j.get<std::uint64_t>();
			const auto type = type_j.as<std::uint32_t>();

			if (type != 3u)
			{
				continue;
			}

			const auto friend_user = database::users::find_from_account(id);
			if (!friend_user.has_value())
			{
				continue;
			}

			const auto header = friend_user->get_loadout_header();
			auto& result_entry = result["header_list"][idx++];
			result_entry["account_id"]["id"] = id;
			result_entry["account_id"]["type"] = type;
			result_entry["current_class"] = header.class_idx;
			result_entry["energy_invested_base"] = header.energy_invested_base;
			result_entry["energy_invested_class"] = header.energy_invested_class;
			result_entry["name_plate"] = header.nameplate;
		}

		return result;
	}
}
