#include <std_include.hpp>

#include "cmd_event_get_ranking.hpp"

#include "database/models/rankings.hpp"

namespace emulator::ssd
{
	json::value cmd_event_get_ranking::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		param_t param{};
		if (!json::read(param, data))
		{
			return error(ERR_INVALIDARG);
		}

		param.num = std::min(param.num, 30u);

		result["my_current_event_point"] = user->get_event_point();
		result["my_disp_rank"] = 0;
		result["my_rank"] = 0;
		result["my_total_event_point"] = user->get_event_point_total();
		result["ranking"] = json::array();
		result["total_number"] = 0;

		const auto self = database::rankings::get_user_entry(user->get_user_id(), database::rankings::ranking_type_event);
		if (self.has_value())
		{
			result["my_disp_rank"] = self->get_rank();
			result["my_rank"] = self->get_rank_number();
		}

		const auto entries = database::rankings::get_entries(database::rankings::ranking_type_event, 0ull, param.num, param.start_rank);
		for (auto i = 0ull; i < entries.size(); i++)
		{
			auto& entry_j = result["ranking"][i];
			auto& loadout_header = entry_j["loadout_header"];
			const auto header = entries[i].get_user_loadout_header();

			loadout_header["account_id"]["id"] = entries[i].get_account_id();
			loadout_header["account_id"]["type"] = 3;
			loadout_header["current_class"] = header.class_idx;
			loadout_header["energy_invested_base"] = header.energy_invested_base;
			loadout_header["energy_invested_class"] = header.energy_invested_class;
			loadout_header["name_plate"] = header.nameplate;

			entry_j["rank"] = entries[i].get_rank_number();
			entry_j["disp_rank"] = entries[i].get_rank();
			entry_j["event_point"] = entries[i].get_points();
			entry_j["flag"] = 0;
		}

		result["total_number"] = database::rankings::get_entry_count(database::rankings::ranking_type_event);

		return result;
	}

	std::uint32_t cmd_event_get_ranking::flags()
	{
		return CMD_NEEDS_USER;
	}
}
