#include <std_include.hpp>

#include "cmd_crew_update.hpp"

namespace emulator::ssd
{
	json::value cmd_crew_update::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		auto& group_level_j = data["group_level"];
		if (!group_level_j.is_object())
		{
			auto crew_levels = std::make_unique<database::players::crew_levels_t>();
			crew_levels->parse(group_level_j);
			user->current_player->set_crew_levels(*crew_levels);
		}

		auto crew_member_list = std::make_unique<database::players::crew_member_list_t>();
		user->current_player->get_crew_member_list(*crew_member_list);

		const auto do_list = [&]<typename T>(const std::string_view& name, void(*accessor)(database::players::crew_member_t&, T&))
		{
			auto& list_j = data[name];
			if (!list_j.is_array())
			{
				return;
			}

			for (auto i = 0ull; i < list_j.size(); i++)
			{
				T entry{};
				if (json::read(entry, list_j[i]))
				{
					continue;
				}

				const auto member = crew_member_list->find_member(entry.unique_id);
				if (member == nullptr)
				{
					continue;
				}

				accessor(*member, entry);
			}
		};

		do_list.template operator()<group_transfer_entry_t>("group_transfer_list", 
			[](database::players::crew_member_t& member, group_transfer_entry_t& entry)
			{
				member.previous_group = member.current_group;
				member.current_group = entry.group_id;
			}
		);

		// todo?
		//do_list.template operator()<update_flag_entry_t>("update_flag_list",
		//	[](database::players::crew_member_t& member, update_flag_entry_t& entry)
		//	{
		//		member.health_flag = entry.flag;
		//	}
		//);
		
		do_list.template operator()<update_nickname_entry_t>("update_nickname_list", 
			[](database::players::crew_member_t& member, update_nickname_entry_t& entry)
			{
				if (entry.name.size() >= sizeof(member.nickname) - 1)
				{
					return;
				}

				std::memset(member.nickname, 0, sizeof(member.nickname));
				snprintf(member.nickname, sizeof(member.nickname), "%s", entry.name.data());
			}
		);

		user->current_player->set_crew_member_list(*crew_member_list);

		return result;
	}

	std::uint32_t cmd_crew_update::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
