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

		auto& personal_reward_j = result["personal_reward"];
		auto& mission_rewards_j = result["reward"];

		personal_reward_j["battle_pack_list"] = json::array();
		personal_reward_j["expire_date"] = 0;
		personal_reward_j["kub_boost_flag"] = static_cast<std::uint8_t>(kub_boost.valid);
		personal_reward_j["nonstackable_list"] = json::array();
		personal_reward_j["present_box_num"] = 0;
		personal_reward_j["present_list"] = json::array();
		personal_reward_j["recipe_list"] = json::array();
		personal_reward_j["resources_list"] = json::array();
		personal_reward_j["stackable_list"] = json::array();
		personal_reward_j["text_id"] = 0;
		personal_reward_j["energy"] = 0;

		const auto rank_count = static_cast<std::uint32_t>(iter->second->rank_threshold.size());

		std::uint8_t rank = 0u;
		for (; rank <= rank_count; rank++)
		{
			if (rank < rank_count && param.score >= iter->second->rank_threshold[rank])
			{
				break;
			}
		}

		result["limit_result"]["mission_code"] = param.mission_id;

		auto& limit_result = result["limit_result"]["limit_result"];
		limit_result = json::array();

		for (auto i = 0u; i < rank_count; i++)
		{
			limit_result[i]["count"] = 1;
			limit_result[i]["max"] = 1;
			limit_result[i]["index"] = i + 1;
			limit_result[i]["rank"] = 0;

			if (i < rank)
			{
				continue;
			}

			if (rank_count == 6 && rank == 0)
			{
				limit_result[i]["rank"] = 10;
			}
			else if (rank_count == 6)
			{
				limit_result[i]["rank"] = std::min(std::uint8_t(5u), rank) - 1;
			}
			else if (rank_count <= 5)
			{
				limit_result[i]["rank"] = i;
			}
		}

		result["reward"] = json::array();

		if (rank_count <= 5)
		{
			result["rank"] = std::min(std::uint8_t(5u), rank);
		}
		else
		{
			if (rank == 0)
			{
				result["rank"] = 10; // S+ rank
			}
			else
			{
				result["rank"] = std::min(std::uint8_t(5u), rank) - 1;
			}
		}

		result["boost_flag"] = has_boost & 1;
		result["reward_event_point"] = 0;

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

		auto record_type = param.event_flag & 1 
			? database::coop_missions::mission_type_event
			: database::coop_missions::mission_type_embedded;

		database::coop_missions::add_result(user->current_player->get_player_id(), record_type, record);

		const auto now = std::chrono::system_clock::now();
		const auto expire_date = now + 14 * 24h;
		const auto expire_date_s = std::chrono::duration_cast<std::chrono::seconds>(expire_date.time_since_epoch());
		const auto text_id = 229691447841577;

		personal_reward_j["text_id"] = text_id;
		personal_reward_j["expire_date"] = expire_date_s;

		std::uint32_t reward_event_point{};

		const auto personal_rewards = database::coop_missions::generate_personal_rewards(iter->second->enemy_level, param.personal_score);
		const auto rewards = database::coop_missions::generate_rewards(iter->second->enemy_level, rank, reward_event_point);

		database::events::cached_event.access([&](const std::optional<database::events::running_event>& event)
		{
			if (!event.has_value() || !event->params->has_mission(iter->second->mission_id))
			{
				reward_event_point = 0u;
			}
		});

		database::players::stackable_item_list_t stackable_list{};
		database::players::inventory_resource_list_t resource_list{};
		database::players::player_inventory_t player_inventory{};
		database::players::battle_pack_list_t battle_pack_list{};

		if (!personal_rewards.empty() || !rewards.empty())
		{
			user->current_player->get_stackable_item_list(stackable_list);
			user->current_player->get_inventory_resource_list(resource_list);
			user->current_player->get_inventory(player_inventory);
			user->current_player->get_battle_pack_list(battle_pack_list);
		}

		for (auto i = rank; i < rank_count; i++)
		{
			auto& rank_entry = mission_rewards_j[i - rank];
			rank_entry["battle_pack_list"] = json::array();
			rank_entry["expire_date"] = expire_date_s.count();
			rank_entry["kub_boost_flag"] = (i * 4u + 2u) + static_cast<std::uint8_t>(kub_boost.valid);
			rank_entry["nonstackable_list"] = json::array();
			rank_entry["present_box_num"] = 0;
			rank_entry["present_list"] = json::array();
			rank_entry["recipe_list"] = json::array();
			rank_entry["resources_list"] = json::array();
			rank_entry["stackable_list"] = json::array();
			rank_entry["text_id"] = text_id;
			rank_entry["energy"] = 0;
		}

		database::users::give_item_params_t give_params{};
		give_params.resource_list = &resource_list;
		give_params.stackable_list = &stackable_list;
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

			if (!user->give_item(give_item, give_params, personal_reward_j))
			{
				auto& present_list_j = personal_reward_j["present_list"];
				presents.emplace_back(give_item);
				give_item.to_json(present_list_j[present_list_j.size()]);
			}
		}

		for (const auto& reward : rewards)
		{
			if (reward.rank < rank)
			{
				continue;
			}

			auto& rank_entry = mission_rewards_j[reward.rank - rank];
			auto& present_list_j = rank_entry["present_list"];

			game::item_t give_item = reward.item;

			if (kub_boost.valid && give_item.category == game::ITEM_CATEGORY_ENERGY)
			{
				// multiplier changes (sometimes) based on enemy level but i cba
				const auto multiplier = kub_boost_scalar * 2.2f;
				give_item.num = static_cast<std::uint32_t>(static_cast<float>(give_item.num) * multiplier);
			}

			if (!user->give_item(give_item, give_params, rank_entry))
			{
				presents.emplace_back(give_item);
				give_item.to_json(present_list_j[present_list_j.size()]);
			}
		}

		if (reward_event_point > 0)
		{
			if (bp_boost.valid)
			{
				reward_event_point *= bp_boost.get_multiplier();
			}

			user->add_event_points(reward_event_point);
			database::rankings::increment_points(user->get_user_id(), database::rankings::ranking_type_event, reward_event_point);
			result["reward_event_point"] = reward_event_point;
		}

		database::present_box::add_items(user->get_user_id(),
			database::present_box::present_flag_expire, expire_date_s, presents, text_id);

		const auto present_count = database::present_box::get_present_count(user->get_user_id());
		for (auto i = rank; i < rank_count; i++)
		{
			auto& rank_entry = mission_rewards_j[i - rank];
			rank_entry["present_box_num"] = present_count;
		}

		if (!personal_rewards.empty() || !rewards.empty())
		{
			user->current_player->set_inventory_resource_list(resource_list);
			user->current_player->set_stackable_item_list(stackable_list);
			user->current_player->set_inventory(player_inventory);
			user->current_player->set_battle_pack_list(battle_pack_list);
		}

		return result;
	}

	std::uint32_t cmd_coop_mission_result::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
