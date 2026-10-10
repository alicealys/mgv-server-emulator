#include <std_include.hpp>

#include "cmd_coop_mission_result.hpp"
#include "cmd_inventory_save.hpp"

#include "database/models/rankings.hpp"
#include "database/models/coop_missions.hpp"

namespace emulator::ssd
{
	cmd_coop_mission_result::cmd_coop_mission_result()
	{
		auto data = utils::resources::load_json(RESOURCE_COOP_MISSION_REWARD_SETTINGS);
		auto& event_point_table_j = data["event_point_table"];

		for (auto& [k, v] : event_point_table_j.get_object())
		{
			const auto difficulty = std::atoi(k.data());
			reward_settings_t::point_table_entry_t entry{};
			if (!json::read(entry, v))
			{
				continue;
			}

			this->reward_settings_.event_point_table.insert(std::make_pair(static_cast<std::uint32_t>(difficulty), entry));
		}
	}

	json::value cmd_coop_mission_result::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		param_t param{};
		if (!json::read(param, data))
		{
			return error(ERR_INVALIDARG);
		}

		const auto iter = game::parameters_table.ssd_ui_coop_mission_info->coop_missions.find(param.mission_id);
		if (iter == game::parameters_table.ssd_ui_coop_mission_info->coop_missions.end())
		{
			return error(ERR_NOT_FOUND);
		}

		cmd_inventory_save::do_save(data, user);

		auto& tips_open_info_j = data["tips_open_info"];
		auto& avatar_condition_j = data["avatar_condition"];
		auto& gimmick_timer_info_j = data["gimmick_timer_info"];
		[[ maybe_unused ]] auto& play_record_save_coop_checkpoint_j = data["play_record_save_coop"];
		[[ maybe_unused ]] auto& play_record_additional_130_coop_checkpoint_j = data["play_record_additional_130_coop"];

		if (gimmick_timer_info_j.is_object())
		{
			auto gimmick_info = user->current_player->get_gimmick_info();
			gimmick_info.timer.parse(gimmick_timer_info_j);
			user->current_player->set_gimmick_info(gimmick_info);
		}

		const auto story_unlock_info = std::make_unique<database::players::story_unlock_info_t>();
		user->current_player->get_story_unlock_info(*story_unlock_info);
		story_unlock_info->set_story_sequence_number(param.story_sequence_number);

		if (tips_open_info_j.is_array() && tips_open_info_j.size())
		{
			story_unlock_info->tips_open_info.parse(tips_open_info_j[0]);
		}

		user->current_player->set_story_unlock_info(*story_unlock_info);

		if (avatar_condition_j.is_array() && avatar_condition_j.size())
		{
			const auto mission_info = std::make_unique<database::players::mission_info_t>();
			user->current_player->get_mission_info(*mission_info);
			mission_info->parse_avatar_condition(avatar_condition_j[0]);
			user->current_player->set_mission_info(*mission_info);
		}

		//if (play_record_save_coop_checkpoint_j.is_array())
		//{
		//	// TODO
		//}
		//
		//if (play_record_additional_130_coop_checkpoint_j.is_array())
		//{
		//	// TODO
		//}

		[[ maybe_unused ]] auto& personal_reward = result["personal_reward"];
		[[ maybe_unused ]] auto& reward = result["reward"];

		personal_reward["battle_pack_list"] = json::array();
		personal_reward["expire_date"] = 0;
		personal_reward["kub_boost_flag"] = 0;
		personal_reward["nonstackable_list"] = json::array();
		personal_reward["present_box_num"] = 0;
		personal_reward["present_list"] = json::array();
		personal_reward["recipe_list"] = json::array();
		personal_reward["resources_list"] = json::array();
		personal_reward["stackable_list"] = json::array();
		personal_reward["text_id"] = 0;
		personal_reward["energy"] = 0;

		std::uint8_t rank = 0u;
		for (; rank < 5u; rank++)
		{
			if (rank < iter->second->rank_threshold.size() && param.score >= iter->second->rank_threshold[rank])
			{
				break;
			}
		}

		result["limit_result"]["limit_result"] = json::array();
		result["limit_result"]["mission_code"] = param.mission_id;
		result["reward"] = json::array();
		result["rank"] = rank;
		result["reward_event_point"] = 0;

		database::coop_missions::coop_mission_record_t record{};
		record.mission_code = iter->second->mission_id;
		record.iris_score = param.score;
		record.personal_score = param.personal_score;
		record.waves = param.waves;
		record.rescue = param.rescue;
		record.clear_rank = rank;

		database::coop_missions::add_result(user->current_player->get_player_id(), database::coop_missions::mission_type_embedded, record);

		if (param.personal_score >= 150)
		{
			const auto now = std::chrono::system_clock::now();
			const auto expire_date = now + 14 * 24h;
			const auto expire_date_s = std::chrono::duration_cast<std::chrono::seconds>(expire_date.time_since_epoch());
			const auto text_id = 229691447841577;

			personal_reward["text_id"] = text_id;
			personal_reward["expire_date"] = expire_date_s;

			// todo: give reward
			// personal_reward ...
			// reward ...

			const auto table_iter = this->reward_settings_.event_point_table.find(iter->second->enemy_level);
			if (table_iter != this->reward_settings_.event_point_table.end())
			{
				const auto reward_event_point = table_iter->second[rank];
				user->add_event_points(reward_event_point);
				database::rankings::increment_points(user->get_user_id(), database::rankings::ranking_type_event, reward_event_point);
				result["reward_event_point"] = reward_event_point;
			}
		}

		return result;
	}

	std::uint32_t cmd_coop_mission_result::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
