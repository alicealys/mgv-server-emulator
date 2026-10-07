#include <std_include.hpp>

#include "cmd_event_reward_receive.hpp"

#include "database/models/events.hpp"
#include "database/models/rankings.hpp"
#include "database/models/present_box.hpp"

namespace emulator::ssd
{
	json::value cmd_event_reward_receive::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		std::uint32_t type{};
		if (!json::read(type, data["type"]))
		{
			return error(ERR_INVALIDARG);
		}

		const auto now = std::chrono::system_clock::now();
		const auto expire_date = now + 14 * 24h;
		const auto expire_date_s = std::chrono::duration_cast<std::chrono::seconds>(expire_date.time_since_epoch());

		auto& reward_info = result["reward_info"];
		reward_info["energy"] = 0;
		reward_info["kub_boost_flag"] = 0;
		reward_info["expire_date"] = 0;
		reward_info["present_box_num"] = 0;
		reward_info["battle_pack_list"] = json::array();
		reward_info["nonstackable_list"] = json::array();
		reward_info["present_list"] = json::array();
		reward_info["recipe_list"] = json::array();
		reward_info["resources_list"] = json::array();
		reward_info["stackable_list"] = json::array();
		reward_info["text_id"] = 0;

		const auto user_inventory = std::make_unique<database::users::user_inventory_t>();
		user->get_inventory(*user_inventory);

		auto& present_list = reward_info["present_list"];

		const auto current_event = database::events::get_current_event();
		if (!current_event.has_value())
		{
			return result;
		}

		const auto add_reward = [&](const game::item_t& item, const std::uint64_t text_id)
		{
			reward_info["text_id"] = text_id;
			reward_info["expire_date"] = expire_date_s.count();

			database::present_box::add_item(user->current_player->get_player_id(),
				database::present_box::present_flag_expire,
				expire_date_s, item, text_id);

			item.to_json(present_list[present_list.size()]);
		};

		switch (type)
		{
		case 0:
		{
			if (!current_event->params->enable_border)
			{
				return result;
			}

			const auto acquired_rewards = database::events::get_acquired_rewards(current_event->get_event_id(), user->get_user_id());
			const auto got_reward = [&](const std::uint8_t type, const std::uint64_t reward_id)
			{
				const auto iter_cb = [&](const database::events::event_reward& reward)
				{
					return reward.get_reward_type() == type && reward.get_reward_id() == reward_id;
				};

				const auto iter = std::ranges::find_if(acquired_rewards.begin(), acquired_rewards.end(), iter_cb);
				return iter != acquired_rewards.end();
			};

			for (auto i = 0ull; i < current_event->params->reward_border_list.size(); i++)
			{
				const auto& reward = current_event->params->reward_border_list[i];
				if (reward.border_point > user->get_event_point_total() || got_reward(database::events::reward_type_border, reward.reward_id))
				{
					continue;
				}

				auto is_permanent = false;
				if (user->has_permanent_item(reward.item_info, *user_inventory, is_permanent))
				{
					continue;
				}

				add_reward(reward.item_info, 1898634391ull);
				database::events::add_reward(current_event->get_event_id(), user->get_user_id(),
					database::events::reward_type_border, reward.reward_id, true);
			}
		}
		case 1:
		{
			const auto entitlements = database::events::get_entitlements(current_event->get_event_id(), user->get_user_id());
			for (const auto& entitlement : entitlements)
			{
				const auto event_params = database::events::get_event_params(entitlement.get_event_type());
				if (event_params == nullptr)
				{
					continue;
				}

				if (entitlement.get_reward_type() != database::events::reward_type_ranking)
				{
					continue;
				}

				const auto iter_cb = [&](const database::events::event_ranking_reward_t& reward)
				{
					return reward.reward_id == entitlement.get_reward_id();
				};

				const auto iter = std::ranges::find_if(event_params->reward_ranking_list.begin(), event_params->reward_ranking_list.end(), iter_cb);
				if (iter == event_params->reward_ranking_list.end())
				{
					continue;
				}

				for (const auto& item : iter->item_info)
				{
					auto is_permanent = false;
					if (user->has_permanent_item(item, *user_inventory, is_permanent))
					{
						continue;
					}

					add_reward(item, 1306356998ull);
				}
			}

			database::events::clear_entitlements(current_event->get_event_id(), user->get_user_id());
		}
		}

		reward_info["present_box_num"] = database::present_box::get_present_count(user->get_user_id());

		return result;
	}

	std::uint32_t cmd_event_reward_receive::flags()
	{
		return CMD_NEEDS_USER;
	}
}
