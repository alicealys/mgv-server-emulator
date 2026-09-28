#include <std_include.hpp>

#include "cmd_crew_use_item.hpp"

#include "database/models/crew_members.hpp"

namespace emulator::ssd
{
	json::value cmd_crew_use_item::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		std::uint64_t unique_id{};
		if (!json::read(unique_id, data["unique_id"]))
		{
			return error(ERR_INVALIDARG);
		}

		auto& list_j = data["item_list"];
		if (!list_j.is_array())
		{
			return error(ERR_INVALIDARG);
		}

		database::players::inventory_resource_list_t resource_list;
		user->current_player->get_inventory_resource_list(resource_list);

		const auto member = database::crew_members::find(user->current_player->get_player_id(), unique_id);
		if (!member.has_value())
		{
			return error(ERR_NOT_FOUND);
		}

		std::array<std::uint32_t, 6> add_counts{};

		for (auto i = 0ull; i < list_j.size(); i++)
		{
			entry_t entry{};
			if (!json::read(entry, list_j[i]))
			{
				continue;
			}

			std::uint32_t count = entry.num;
			if (!resource_list.spend_resource(entry.item_resource_id, count))
			{
				return error(ERR_RESOURCE_SHORTAGE);
			}

			std::uint32_t* target = nullptr;
			switch (entry.item_resource_id)
			{
				case game::RES_Crew_Growth_develop:
					target = &add_counts[0];
					break;
				case game::RES_Crew_Growth_food:
					target = &add_counts[1];
					break;
				case game::RES_Crew_Growth_medical:
					target = &add_counts[2];
					break;
				case game::RES_Crew_Growth_farm:
					target = &add_counts[3];
					break;
				case game::RES_Crew_Growth_defense:
					target = &add_counts[4];
					break;
				case game::RES_Crew_Growth_explore:
					target = &add_counts[5];
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

		member->add_items(add_counts);

		user->current_player->set_inventory_resource_list(resource_list);

		return result;
	}

	std::uint32_t cmd_crew_use_item::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
