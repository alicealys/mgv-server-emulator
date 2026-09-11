#include <std_include.hpp>

#include "cmd_building_repair.hpp"

namespace emulator::ssd
{
	nlohmann::json cmd_building_repair::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;

		auto& map_location_j = data["map_location"];
		auto& current_life_obj_j = data["current_life"];
		auto& edge_type_j = data["edge_type"];
		auto& production_id_j = data["production_id"];

		if (!map_location_j.is_number_unsigned() || !current_life_obj_j.is_object() ||
			!edge_type_j.is_number_unsigned() || !production_id_j.is_number_unsigned())
		{
			return error(ERR_INVALIDARG);
		}

		const auto map_location = map_location_j.get<std::uint32_t>();
		const auto edge_type = edge_type_j.get<std::uint32_t>();
		const auto production_id = production_id_j.get<std::uint16_t>();

		if (map_location > 1 || edge_type >= database::players::edge_type_count)
		{
			return error(ERR_INVALIDARG);
		}

		auto& column_j = current_life_obj_j["column"];
		auto& row_j = current_life_obj_j["row"];
		auto& current_life_j = current_life_obj_j["current_life"];

		if (!column_j.is_number_unsigned() || !row_j.is_number_unsigned() || 
			!current_life_j.is_number_unsigned())
		{
			return error(ERR_INVALIDARG);
		}

		const auto col = column_j.get<std::uint32_t>();
		const auto row = row_j.get<std::uint32_t>();
		const auto current_life = current_life_j.get<std::uint16_t>();

		const auto building_info = std::make_unique<database::players::building_info_t>();
		user->current_player->get_building_info(*building_info, map_location);

		building_info->cells[row][col].edges[edge_type].life = current_life;
		building_info->cells[row][col].edges[edge_type].production_index = production_id;

		user->current_player->set_building_info(*building_info, map_location);

		return result;
	}

	std::uint32_t cmd_building_repair::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
