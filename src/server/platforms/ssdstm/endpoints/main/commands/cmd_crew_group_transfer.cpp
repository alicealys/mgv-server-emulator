#include <std_include.hpp>

#include "cmd_crew_group_transfer.hpp"

namespace emulator::ssd
{
	json::value cmd_crew_group_transfer::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		auto& list_j = data["group_transfer_list"];
		if (!list_j.is_array())
		{
			return error(ERR_INVALIDARG);
		}

		auto crew_member_list = std::make_unique<database::players::crew_member_list_t>();
		user->current_player->get_crew_member_list(*crew_member_list);

		for (auto i = 0ull; i < list_j.size(); i++)
		{
			entry_t entry{};
			if (!json::read(entry, list_j[i]))
			{
				continue;
			}

			const auto member = crew_member_list->find_member(entry.unique_id);
			if (member == nullptr)
			{
				continue;
			}

			member->previous_group = member->current_group;
			member->current_group = entry.group_id;
		}

		user->current_player->set_crew_member_list(*crew_member_list);

		return result;
	}

	std::uint32_t cmd_crew_group_transfer::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
