#include <std_include.hpp>

#include "cmd_crew_use_item.hpp"

namespace emulator::ssd
{
	json::value cmd_crew_use_item::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		std::uint32_t unique_id{};
		if (!json::read(unique_id, data["unique_id"]))
		{
			return error(ERR_INVALIDARG);
		}

		auto& list_j = data["item_list"];
		if (!list_j.is_array())
		{
			return error(ERR_INVALIDARG);
		}

		const auto crew_member_list = std::make_unique<database::players::crew_member_list_t>();
		const auto resource_list = std::make_unique<database::players::inventory_resource_list_t>();
		user->current_player->get_inventory_resource_list(*resource_list);
		user->current_player->get_crew_member_list(*crew_member_list);

		const auto member = crew_member_list->find_member(unique_id);
		if (member == nullptr)
		{
			return error(ERR_NOT_FOUND);
		}

		for (auto i = 0ull; i < list_j.size(); i++)
		{
			entry_t entry{};
			if (!json::read(entry, list_j[i]))
			{
				continue;
			}

			std::uint32_t count = entry.num;
			if (!resource_list->spend_resource(entry.item_resource_id, count))
			{
				return error(ERR_RESOURCE_SHORTAGE);
			}

			std::uint16_t* target = nullptr;
			switch (entry.item_resource_id)
			{
				case game::RES_Crew_Growth_develop:
					target = &member->item1_count;
					break;
				case game::RES_Crew_Growth_food:
					target = &member->item2_count;
					break;
				case game::RES_Crew_Growth_medical:
					target = &member->item3_count;
					break;
				case game::RES_Crew_Growth_farm:
					target = &member->item4_count;
					break;
				case game::RES_Crew_Growth_defense:
					target = &member->item5_count;
					break;
				case game::RES_Crew_Growth_explore:
					target = &member->item6_count;
					break;
			}

			if (target == nullptr)
			{
				return error(ERR_INVALIDARG);
			}

			if (*target + entry.num > 255)
			{
				return error(ERR_RESULT_OUTOFRANGE);
			}

			*target += entry.num;
		}

		user->current_player->set_inventory_resource_list(*resource_list);
		user->current_player->set_crew_member_list(*crew_member_list);

		return result;
	}

	std::uint32_t cmd_crew_use_item::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
