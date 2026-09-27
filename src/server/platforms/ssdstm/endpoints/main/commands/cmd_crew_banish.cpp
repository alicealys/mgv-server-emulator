#include <std_include.hpp>

#include "cmd_crew_banish.hpp"

namespace emulator::ssd
{
	json::value cmd_crew_banish::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		auto& list_j = data["unique_id_list"];
		if (!list_j.is_array())
		{
			return error(ERR_INVALIDARG);
		}

		auto crew_member_list = std::make_unique<database::players::crew_member_list_t>();
		user->current_player->get_crew_member_list(*crew_member_list);

		for (auto i = 0ull; i < list_j.size(); i++)
		{
			std::uint32_t unique_id{};
			if (!json::read(unique_id, list_j[i]))
			{
				continue;
			}

			const auto member = crew_member_list->find_member(unique_id);
			if (member == nullptr)
			{
				continue;
			}

			std::memset(member, 0, sizeof(database::players::crew_member_t));
		}

		user->current_player->set_crew_member_list(*crew_member_list);

		return result;
	}

	std::uint32_t cmd_crew_banish::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
