#include <std_include.hpp>

#include "cmd_event_get_info.hpp"

#include "database/models/events.hpp"
#include "database/models/rankings.hpp"

namespace emulator::ssd
{
	json::value cmd_event_get_info::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		result["event_list"] = json::array();
		result["season_flag"] = 0;

		const auto current_event = database::events::get_current_event();
		if (!current_event.has_value())
		{
			return result;
		}

		auto& entry = result["event_list"][0];
		current_event->params->to_json(entry);

		entry["reward_start_date"] = current_event->get_start_date().count();
		entry["reward_end_date"] = current_event->get_end_date().count();
		entry["start_date"] = current_event->get_start_date().count();
		entry["end_date"] = current_event->get_end_date().count();
		entry["event_id"] = current_event->get_event_id();

		entry["current_event_point"] = user->get_event_point();
		entry["total_event_point"] = user->get_event_point_total();
		entry["disp_rank"] = 0;
		entry["rank"] = 0;

		if (current_event->params->enable_ranking)
		{
			const auto ranking = database::rankings::get_user_entry(user->get_user_id(), database::rankings::ranking_type_event);
			if (ranking.has_value())
			{
				entry["disp_rank"] = ranking->get_rank();
				entry["rank"] = ranking->get_rank_number();
			}
		}

		if (current_event->params->enable_catalog)
		{
			const auto user_inventory = std::make_unique<database::users::user_inventory_t>();
			user->get_inventory(*user_inventory);

			auto acquired_counts = database::events::get_acquired_reward_counts(current_event->get_event_id(), 
				user->get_user_id(), database::events::reward_type_catalog);

			for (auto i = 0ull; i < current_event->params->reward_catalog_list.size(); i++)
			{
				const auto& reward = current_event->params->reward_catalog_list[i];

				auto& purchase_num = entry["reward_catalog_list"][i]["purchased_num"];
				purchase_num = 0;

				auto is_permanent_item = false;
				if (user->has_permanent_item(reward.item_info, *user_inventory, is_permanent_item))
				{
					purchase_num = 1;
				}
				else if (!is_permanent_item && reward.purchase_limit != 0)
				{
					purchase_num = acquired_counts[reward.reward_id];
				}
			}
		}

		return result;
	}

	std::uint32_t cmd_event_get_info::flags()
	{
		return CMD_NEEDS_USER;
	}
}
