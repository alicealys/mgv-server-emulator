#include <std_include.hpp>

#include "cmd_crew_treat.hpp"

#include "database/models/crew_members.hpp"

namespace emulator::ssd
{
	json::value cmd_crew_treat::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		std::uint32_t option{};
		if (!json::read(option, data["option"]))
		{
			return error(ERR_INVALIDARG);
		}

		auto& list_j = data["treat_list"];
		if (!list_j.is_array())
		{
			return error(ERR_INVALIDARG);
		}

		database::players::stackable_item_list_t item_list;
		user->current_player->get_stackable_item_list(item_list);

		const auto do_treatment = [&](const std::uint32_t item_id, std::uint32_t& target_treatment_id, std::uint32_t& target_treatment_time)
		{
			if (item_id == 0)
			{
				return false;
			}

			const auto iter = game::parameters_table.ssd_sbm_parameters->productions.find(item_id);
			if (iter == game::parameters_table.ssd_sbm_parameters->productions.end())
			{
				return false;
			}

			const auto item = item_list.find_item(item_id);
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
			if (!json::read(entry, list_j[i]))
			{
				continue;
			}

			const auto member = database::crew_members::find(user->current_player->get_player_id(), entry.unique_id);
			if (!member.has_value())
			{
				continue;
			}

			auto injury_id_1 = member->get_injury_id_1();
			auto injury_id_2 = member->get_injury_id_2();
			auto injury_time_1 = member->get_injury_time_1();
			auto injury_time_2 = member->get_injury_time_2();

			if (do_treatment(entry.treat_item_injury_1, injury_id_1, injury_id_2) || 
				do_treatment(entry.treat_item_injury_2, injury_time_1, injury_time_2))
			{
				member->update_injury(injury_id_1, injury_id_2, injury_time_1, injury_time_2);
			}

			auto sickness_id_1 = member->get_sickness_id_1();
			auto sickness_id_2 = member->get_sickness_id_2();
			auto sickness_time_1 = member->get_sickness_time_1();
			auto sickness_time_2 = member->get_sickness_time_2();
			
			if (do_treatment(entry.treat_item_sickness_1, sickness_id_1, sickness_time_2) || 
				do_treatment(entry.treat_item_sickness_2, sickness_id_2, sickness_time_2))
			{
				member->update_sickness(sickness_id_1, sickness_id_2, sickness_time_1, sickness_time_2);
			}
		}

		user->current_player->set_stackable_item_list(item_list);

		return result;
	}

	std::uint32_t cmd_crew_treat::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
