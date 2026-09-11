#include <std_include.hpp>

#include "cmd_building_relocate.hpp"

namespace emulator::ssd
{
	nlohmann::json cmd_building_relocate::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;

		auto& map_location_j = data["map_location"];
		auto& departure_coordinate_j = data["departure_coordinate"];
		auto& departure_edge_type_j = data["departure_edge_type"];
		auto& destination_coordinate_j = data["destination_coordinate"];
		auto& destination_edge_type_j = data["destination_edge_type"];
		auto& destination_rotation_j = data["destination_rotation"];

		if (!map_location_j.is_number_unsigned() || 
			!departure_coordinate_j.is_object() || !departure_edge_type_j.is_number_unsigned() || 
			!destination_coordinate_j.is_object() || !destination_edge_type_j.is_number_unsigned() ||
			!destination_rotation_j.is_number_unsigned())
		{
			return error(ERR_INVALIDARG);
		}

		const auto map_location = map_location_j.get<std::uint32_t>();
		if (map_location > 1)
		{
			return error(ERR_INVALIDARG);
		}

		const auto departure_edge_type = departure_edge_type_j.get<std::uint32_t>();
		const auto destination_edge_type = destination_edge_type_j.get<std::uint32_t>();
		const auto destination_rotation = destination_rotation_j.get<std::uint8_t>();

		if (departure_edge_type >= database::players::edge_type_count || destination_edge_type >= database::players::edge_type_count)
		{
			return error(ERR_INVALIDARG);
		}

		const auto get_coordinate = [](nlohmann::json& data, std::uint32_t& col, std::uint32_t& row)
		{
			auto& column_j = data["column"];
			auto& row_j = data["row"];

			if (!column_j.is_number_unsigned() || !row_j.is_number_unsigned())
			{
				return false;
			}

			col = column_j.get<std::uint32_t>();
			row = row_j.get<std::uint32_t>();

			return col < database::players::building_grid_size && row < database::players::building_grid_size;
		};

		std::uint32_t src_col{}, src_row{};
		std::uint32_t dst_col{}, dst_row{};

		if (!get_coordinate(departure_coordinate_j, src_col, src_row) || 
			!get_coordinate(destination_coordinate_j, dst_col, dst_row))
		{
			return error(ERR_INVALIDARG);
		}

		const auto building_info = std::make_unique<database::players::building_info_t>();
		user->current_player->get_building_info(*building_info, map_location);

		auto& src_edge = building_info->cells[src_row][src_col].edges[departure_edge_type];
		auto& dst_edge = building_info->cells[dst_row][dst_col].edges[destination_edge_type];

		if (src_edge.life == 0u || dst_edge.life != 0u)
		{
			return error(ERR_INVALIDARG);
		}

		std::memcpy(&dst_edge, &src_edge, sizeof(database::players::building_info_t::cell_edge_t));
		std::memset(&src_edge, 0, sizeof(database::players::building_info_t::cell_edge_t));

		dst_edge.rotation = destination_rotation;

		user->current_player->set_building_info(*building_info, map_location);

		return result;
	}

	std::uint32_t cmd_building_relocate::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
