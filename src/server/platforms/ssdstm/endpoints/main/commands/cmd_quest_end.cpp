#include <std_include.hpp>

#include "cmd_quest_end.hpp"

// not implemented
namespace emulator::ssd
{
	json::value cmd_quest_end::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		auto& mission_info_j = data["current_mission_info"];
		auto& quest_record_info_j = data["quest_record_info"];
		auto& story_sequence_number_j = data["story_sequence_number"];

		if (mission_info_j.is_object())
		{
			const auto mission_info = std::make_unique<database::players::mission_info_t>();
			user->current_player->get_mission_info(*mission_info);

			if (!mission_info->parse(mission_info_j))
			{
				return error(ERR_INVALIDARG);
			}

			user->current_player->set_mission_info(*mission_info);
		}

		if (quest_record_info_j.is_array())
		{
			for (auto i = 0ull; i < quest_record_info_j.size(); i++)
			{
				auto quest_record_list = user->current_player->get_quest_record_list();
				quest_record_list.parse_diff(quest_record_info_j);
				user->current_player->set_quest_record_list(quest_record_list);
			}
		}

		if (story_sequence_number_j.is_number())
		{
			const auto story_unlock_info = std::make_unique<database::players::story_unlock_info_t>();
			user->current_player->get_story_unlock_info(*story_unlock_info);
			story_unlock_info->set_story_sequence_number(story_sequence_number_j.as<std::uint32_t>());
			user->current_player->set_story_unlock_info(*story_unlock_info);
		}

		return result;
	}

	std::uint32_t cmd_quest_end::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
