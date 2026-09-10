#include <std_include.hpp>

#include "cmd_mission_end.hpp"

#include "database/models/players.hpp"

namespace emulator::ssd
{
	nlohmann::json cmd_mission_end::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;

		auto& current_mission_info_j = data["current_mission_info"];
		auto& story_unlock_info_j = data["story_unlock_info"];
		auto& mission_record_info_j = data["mission_record_info"];
		auto& open_list_j = data["open_list"];

		if (story_unlock_info_j.is_object())
		{
			const auto story_unlock_info = std::make_unique<database::players::story_unlock_info_t>();
			user->current_player->get_story_unlock_info(*story_unlock_info);
			story_unlock_info->parse(story_unlock_info_j);
			user->current_player->set_story_unlock_info(*story_unlock_info);
		}

		if (current_mission_info_j.is_object())
		{
			const auto mission_info = std::make_unique<database::players::mission_info_t>();
			mission_info->story_sequence_number = 0u;
			mission_info->parse(current_mission_info_j);
			user->current_player->set_mission_info(*mission_info);
		}

		if (mission_record_info_j.is_object())
		{
			const auto mission_record_list = std::make_unique<database::players::mission_record_list_t>();
			user->current_player->get_mission_record_list(*mission_record_list);
			database::players::mission_record_t record{};
			record.parse(mission_record_info_j);
			mission_record_list->try_add_item(record);
			user->current_player->set_mission_record_list(*mission_record_list);
		}

		if (open_list_j.is_object())
		{
			auto& mission_code_list_j = open_list_j["mission_code_list"];
			if (mission_code_list_j.is_array())
			{
				const auto mission_record_list = std::make_unique<database::players::mission_record_list_t>();
				user->current_player->get_mission_record_list(*mission_record_list);

				for (auto i = 0ull; i < mission_code_list_j.size(); i++)
				{
					auto& mission_code_j = mission_code_list_j[i];
					if (!mission_code_j.is_number_unsigned())
					{
						continue;
					}

					const auto mission_code = mission_code_j.get<std::uint32_t>();
					mission_record_list->open_mission(mission_code);
				}

				user->current_player->set_mission_record_list(*mission_record_list);
			}
		}

		// TODO
		result["reward"]["energy"] = 0;
		result["reward"]["kub_boost_flag"] = 0;
		result["reward"]["present_box_num"] = 0;
		result["reward"]["battle_pack_list"] = nlohmann::json::array();
		result["reward"]["nonstackable_list"] = nlohmann::json::array();
		result["reward"]["present_list"] = nlohmann::json::array();
		result["reward"]["recipe_list"] = nlohmann::json::array();
		result["reward"]["resources_list"] = nlohmann::json::array();
		result["reward"]["stackable_list"] = nlohmann::json::array();
		result["reward"]["text_id"] = 0;

		return result;
	}

	std::uint32_t cmd_mission_end::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
