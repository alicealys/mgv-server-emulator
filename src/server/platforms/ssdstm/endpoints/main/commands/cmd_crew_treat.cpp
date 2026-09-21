#include <std_include.hpp>

#include "cmd_crew_treat.hpp"

namespace emulator::ssd
{
	json::value cmd_crew_treat::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		std::uint32_t option{};
		if (json::read(option, data["option"]))
		{
			return error(ERR_INVALIDARG);
		}

		auto& list_j = data["treat_list"];
		if (!list_j.is_array())
		{
			return error(ERR_INVALIDARG);
		}

		const auto item_list = std::make_unique<database::players::stackable_item_list_t>();
		const auto crew_member_list = std::make_unique<database::players::crew_member_list_t>();

		user->current_player->get_crew_member_list(*crew_member_list);
		user->current_player->get_stackable_item_list(*item_list);

		const auto do_treatment = [&](const std::uint32_t item_id, std::uint32_t& target_treatment_id, std::uint32_t& target_treatment_time)
		{
			if (item_id == 0)
			{
				return true;
			}

			const auto iter = game::parameters_table.ssd_sbm_parameters->productions.find(item_id);
			if (iter == game::parameters_table.ssd_sbm_parameters->productions.end())
			{
				return false;
			}

			const auto item = item_list->find_item(item_id);
			if (item == nullptr)
			{
				return false;
			}

			if (item->count <= 0)
			{
				return false;
			}

			item->count -= 1;
			target_treatment_id = 0;
			target_treatment_time = 0;

			// TODO: life?

			return true;
		};

		for (auto i = 0ull; i < list_j.size(); i++)
		{
			entry_t entry{};
			if (json::read(entry, list_j[i]))
			{
				continue;
			}

			const auto member = crew_member_list->find_member(entry.unique_id);
			if (member == nullptr)
			{
				continue;
			}
			
			if (!do_treatment(entry.treat_item_injury_1, member->injury_id_1, member->injury_time_1) || 
				!do_treatment(entry.treat_item_injury_2, member->injury_id_2, member->injury_time_2) ||
				!do_treatment(entry.treat_item_sickness_1, member->sickness_id_1, member->sickness_time_2) || 
				!do_treatment(entry.treat_item_sickness_2, member->sickness_id_2, member->sickness_time_2))
			{
				return error(ERR_INVALIDARG);
			}
		}

		user->current_player->set_crew_member_list(*crew_member_list);
		user->current_player->set_stackable_item_list(*item_list);

		return result;
	}

	std::uint32_t cmd_crew_treat::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
