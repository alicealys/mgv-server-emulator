#include <std_include.hpp>

#include "cmd_event_catalog_reward_purchase.hpp"

#include "database/models/events.hpp"
#include "database/models/present_box.hpp"

namespace emulator::ssd
{
	json::value cmd_event_catalog_reward_purchase::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		std::uint32_t catalog_id{};
		if (!json::read(catalog_id, data["catalog_id"]))
		{
			return error(ERR_INVALIDARG);
		}

		const auto current_event = database::events::get_current_event();
		if (!current_event.has_value() || !current_event->params->enable_catalog)
		{
			return error(ERR_NOT_FOUND);
		}

		const auto iter_cb = [&](const database::events::event_catalog_reward_t& reward)
		{
			return reward.reward_id == catalog_id;
		};

		const auto iter = std::ranges::find_if(current_event->params->reward_catalog_list.begin(), 
			current_event->params->reward_catalog_list.end(), iter_cb);

		if (iter == current_event->params->reward_catalog_list.end())
		{
			return error(ERR_NOT_FOUND);
		}

		const auto user_inventory = std::make_unique<database::users::user_inventory_t>();
		user->get_inventory(*user_inventory);

		auto is_permanent = false;
		if (user->has_permanent_item(iter->item_info, *user_inventory, is_permanent))
		{
			return error(ERR_OVER_CAPACITY);
		}

		if (!is_permanent && iter->purchase_limit != 0)
		{
			const auto acquired_count = database::events::get_acquired_reward_count(current_event->get_event_id(), user->get_user_id(),
				database::events::reward_type_catalog, iter->reward_id);

			if (acquired_count >= iter->purchase_limit)
			{
				return error(ERR_OVER_CAPACITY);
			}
		}

		if (!user->spend_event_points(iter->price_point))
		{
			return error(ERR_SHORTAGE);
		}

		const auto now = std::chrono::system_clock::now();
		const auto expire_date = now + 14 * 24h;
		const auto expire_date_s = std::chrono::duration_cast<std::chrono::seconds>(expire_date.time_since_epoch());

		const auto present_id = database::present_box::add_item(user->get_user_id(),
			database::present_box::present_flag_expire, 
			expire_date_s, iter->item_info, 964553496ull);

		if (present_id == 0ull)
		{
			user->add_event_points(iter->price_point);
			return error(ERR_DATABASE);
		}

		database::events::add_reward(current_event->get_event_id(), user->get_user_id(), database::events::reward_type_catalog, iter->reward_id, true);

		result["remaining_point"] = database::users::get_event_points(user->get_user_id());
		result["present_id"] = present_id;

		return result;
	}

	std::uint32_t cmd_event_catalog_reward_purchase::flags()
	{
		return CMD_NEEDS_USER;
	}
}
