#include <std_include.hpp>

#include "cmd_coop_mission_result.hpp"
#include "cmd_inventory_save.hpp"

#include "database/models/rankings.hpp"
#include "database/models/coop_missions.hpp"
#include "database/models/boosts.hpp"
#include "database/models/present_box.hpp"
#include "database/models/challenge_tasks.hpp"
#include "database/models/events.hpp"

namespace emulator::ssd
{
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
		auto& play_record_save_coop_checkpoint_j = data["play_record_save_coop"];
		auto& play_record_additional_130_coop_checkpoint_j = data["play_record_additional_130_coop"];

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

		if (play_record_save_coop_checkpoint_j.is_array() && play_record_additional_130_coop_checkpoint_j.is_array())
		{
			const auto play_record = std::make_unique<database::players::player_play_record_t>();
			user->current_player->get_play_record(*play_record);
			play_record->parse_coop_save(play_record_save_coop_checkpoint_j, play_record_additional_130_coop_checkpoint_j);
			user->current_player->set_play_record(*play_record);
		}

		database::challenge_tasks::send_progress_list(user->current_player->get_player_id(), data["order_progress_list"]);

		const auto boost_list = database::boosts::get_boost_list(user->get_user_id());
		const auto& kub_boost = boost_list[database::boosts::boost_type_kub];
		const auto& bp_boost = boost_list[database::boosts::boost_type_bp];
		const auto has_boost = bp_boost.valid || kub_boost.valid;

		auto kub_boost_scalar = 1.f;
		if (kub_boost.valid)
		{
			kub_boost_scalar = (static_cast<float>(kub_boost.get_multiplier()) / 2.f);
		}

		const auto rank_count = static_cast<std::uint8_t>(iter->second->rank_threshold.size());
		const auto rank = database::coop_missions::calc_rank(*iter->second, param.score);
		const auto converted_rank = database::coop_missions::convert_rank(*iter->second, rank);

		auto& limit_result_j = result["limit_result"];
		auto& personal_reward_j = result["personal_reward"];
		auto& mission_rewards_j = result["reward"];
		auto& reward_event_point_j = result["reward_event_point"];

		database::users::reward_info_t personal_reward_info{};
		personal_reward_info.kub_boost_flag = static_cast<std::uint8_t>(kub_boost.valid);

		limit_result_j["mission_code"] = param.mission_id;
		limit_result_j["limit_result"] = json::array();
		result["rank"] = converted_rank;
		result["boost_flag"] = has_boost & 1;
		mission_rewards_j = json::array();
		reward_event_point_j = 0;

		const auto& reward_settings = database::coop_missions::get_reward_settings();
		if (param.clear_type != 0 && param.personal_score < reward_settings.min_personal_score)
		{
			return result;
		}

		database::coop_missions::coop_mission_record_t record{};
		record.mission_code = iter->second->mission_id;
		record.iris_score = param.score;
		record.personal_score = param.personal_score;
		record.waves = param.waves;
		record.rescue = param.rescue;
		record.clear_rank = rank;

		const auto record_type = param.event_flag & 1 
			? database::coop_missions::mission_type_event
			: database::coop_missions::mission_type_embedded;

		database::coop_missions::add_result(user->current_player->get_player_id(), record_type, record);

		std::uint32_t reward_event_point{};

		const auto personal_rewards = database::coop_missions::generate_personal_rewards(iter->second->enemy_level, param.personal_score);
		const auto rewards = database::coop_missions::generate_rewards(*iter->second, rank, reward_event_point);

		database::events::cached_event.access([&](const std::optional<database::events::running_event>& event)
		{
			if (!event.has_value() || !event->params->has_mission(iter->second->mission_id))
			{
				reward_event_point = 0u;
			}
		});

		if (reward_event_point > 0)
		{
			if (bp_boost.valid)
			{
				reward_event_point *= bp_boost.get_multiplier();
			}

			user->add_event_points(reward_event_point);
			database::rankings::increment_points(user->get_user_id(), database::rankings::ranking_type_event, reward_event_point);
			reward_event_point_j = reward_event_point;
		}

		if (personal_rewards.empty() || rewards.empty())
		{
			personal_reward_info.to_json(personal_reward_j);
			return result;
		}

		const auto expire_date = utils::time::from_now(14d);
		constexpr const auto text_id = game::lang_id_mission_completion_reward;

		personal_reward_info.text_id = text_id;
		personal_reward_info.expire_date = expire_date;

		database::players::stackable_item_list_t stackable_list{};
		database::players::nonstackable_item_list_t nonstackable_list{};
		database::players::inventory_resource_list_t resource_list{};
		database::players::player_inventory_t player_inventory{};
		database::players::battle_pack_list_t battle_pack_list{};

		user->current_player->get_stackable_item_list(stackable_list);
		user->current_player->get_nonstackable_item_list(nonstackable_list);
		user->current_player->get_inventory_resource_list(resource_list);
		user->current_player->get_inventory(player_inventory);
		user->current_player->get_battle_pack_list(battle_pack_list);

		std::vector<database::users::reward_info_t> rank_reward_infos(rank_count);

		database::users::give_item_params_t give_params{};
		give_params.resource_list = &resource_list;
		give_params.stackable_list = &stackable_list;
		give_params.nonstackable_list = &nonstackable_list;
		give_params.player_inventory = &player_inventory;
		give_params.battle_pack_list = &battle_pack_list;
		give_params.text_id = text_id;

		std::vector<game::item_t> presents;

		for (auto& reward : personal_rewards)
		{
			game::item_t give_item = reward.item;
			if (kub_boost.duration && give_item.category == game::ITEM_CATEGORY_ENERGY)
			{
				const auto multiplier = kub_boost_scalar * 1.1f;
				give_item.num = static_cast<std::uint32_t>(static_cast<float>(give_item.num) * multiplier);
			}

			if (!user->give_item(give_item, give_params, &personal_reward_info))
			{
				personal_reward_info.present_list.emplace_back(give_item);
				presents.emplace_back(give_item);
			}
		}

		for (const auto& reward : rewards)
		{
			if (reward.rank >= rank_reward_infos.size())
			{
				continue;
			}

			auto& rank_entry = rank_reward_infos[reward.rank];
			rank_entry.expire_date = expire_date;
			rank_entry.kub_boost_flag = (reward.rank * 4u + 2u) + static_cast<std::uint8_t>(kub_boost.valid);
			rank_entry.text_id = text_id;

			game::item_t give_item = reward.item;
			if (kub_boost.valid && give_item.category == game::ITEM_CATEGORY_ENERGY)
			{
				// multiplier changes (sometimes) based on enemy level but i cba
				const auto multiplier = kub_boost_scalar * 2.2f;
				give_item.num = static_cast<std::uint32_t>(static_cast<float>(give_item.num) * multiplier);
			}

			if (!user->give_item(give_item, give_params, &rank_entry))
			{
				rank_entry.present_list.emplace_back(give_item);
				presents.emplace_back(give_item);
			}
		}

		database::present_box::add_items(user->get_user_id(),
			database::present_box::present_flag_expire, 
			expire_date, presents, text_id);

		personal_reward_info.to_json(personal_reward_j);

		const auto present_count = database::present_box::get_present_count(user->get_user_id());
		for (auto i = 0ull; i < rank_reward_infos.size(); i++)
		{
			auto& info = rank_reward_infos[i];
			if (info.text_id != 0)
			{
				info.present_box_num = present_count;
				rank_reward_infos[i].to_json(mission_rewards_j[mission_rewards_j.size()]);
			}
		}

		user->current_player->set_inventory_resource_list(resource_list);
		user->current_player->set_stackable_item_list(stackable_list);
		user->current_player->set_nonstackable_item_list(nonstackable_list);
		user->current_player->set_inventory(player_inventory);
		user->current_player->set_battle_pack_list(battle_pack_list);

		return result;
	}

	std::uint32_t cmd_coop_mission_result::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
