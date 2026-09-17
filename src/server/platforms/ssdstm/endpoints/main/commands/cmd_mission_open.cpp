#include <std_include.hpp>

#include "cmd_mission_open.hpp"

namespace emulator::ssd
{
	json::value cmd_mission_open::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		auto& open_list_j = data["open_list"];
		if (!open_list_j.is_object())
		{
			return error(ERR_INVALIDARG);
		}

		auto& mission_code_list_j = open_list_j["mission_code_list"];
		if (mission_code_list_j.is_array())
		{
			const auto mission_record_list = std::make_unique<database::players::mission_record_list_t>();
			user->current_player->get_mission_record_list(*mission_record_list, mission_code_list_j.size());

			for (auto i = 0ull; i < mission_code_list_j.size(); i++)
			{
				auto& mission_code_j = mission_code_list_j[i];
				if (!mission_code_j.is_uint64())
				{
					continue;
				}

				const auto mission_code = mission_code_j.as<std::uint32_t>();
				mission_record_list->open_mission(mission_code);
			}

			user->current_player->set_mission_record_list(*mission_record_list);
		}

		auto& quest_mission_code_list_j = open_list_j["quest_mission_code_list"];
		if (quest_mission_code_list_j.is_array())
		{
			const auto quest_record_list = std::make_unique<database::players::quest_record_list_t>();
			user->current_player->get_quest_record_list(*quest_record_list, quest_mission_code_list_j.size());
			quest_record_list->parse_diff(quest_mission_code_list_j);
			user->current_player->set_quest_record_list(*quest_record_list);
		}

        return result;
	}

	std::uint32_t cmd_mission_open::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
