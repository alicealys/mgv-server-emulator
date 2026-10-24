#include <std_include.hpp>

#include "cmd_event_reward_receive.hpp"

#include "database/models/events.hpp"
#include "database/models/rankings.hpp"
#include "database/models/present_box.hpp"

#include "utils/time.hpp"

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

		const auto expire_date = utils::time::from_now(14d);

		database::users::reward_info_t reward_info{};

		const auto user_inventory = std::make_unique<database::users::user_inventory_t>();
		user->get_inventory(*user_inventory);

		const auto current_event = database::events::get_current_event();
		if (!current_event.has_value())
		{
			return result;
		}

		std::vector<game::item_t> presents;

		const auto add_reward = [&](const game::item_t& item, const std::uint64_t text_id)
		{
			reward_info.text_id = text_id;
			reward_info.expire_date = expire_date;
			reward_info.present_list.emplace_back(item);
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
			constexpr const auto text_id = game::lang_id_total_battle_points_reward;

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

				add_reward(reward.item_info, text_id);
				database::events::add_reward(current_event->get_event_id(), user->get_user_id(),
					database::events::reward_type_border, reward.reward_id, true);
			}

			database::present_box::add_items(user->get_user_id(),
				database::present_box::present_flag_expire,
				expire_date, reward_info.present_list, text_id);
		}
		case 1:
		{
			const auto entitlements = database::events::get_entitlements(current_event->get_event_id(), user->get_user_id());
			constexpr const auto text_id = game::lang_id_ranking_reward;

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

					add_reward(item, text_id);
				}
			}

			database::present_box::add_items(user->get_user_id(),
				database::present_box::present_flag_expire,
				expire_date, reward_info.present_list, text_id);

			database::events::clear_entitlements(current_event->get_event_id(), user->get_user_id());
		}
		}

		reward_info.present_box_num = database::present_box::get_present_count(user->get_user_id());
		reward_info.to_json(result["reward_info"]);

		return result;
	}

	std::uint32_t cmd_event_reward_receive::flags()
	{
		return CMD_NEEDS_USER;
	}
}
