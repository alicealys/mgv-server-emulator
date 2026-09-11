#include <std_include.hpp>

#include "cmd_building_remove.hpp"

namespace emulator::ssd
{
	nlohmann::json cmd_building_remove::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;

		auto& map_location_j = data["map_location"];
		if (!map_location_j.is_number_unsigned())
		{
			return error(ERR_INVALIDARG);
		}

		const auto map_location = map_location_j.get<std::uint32_t>();
		if (map_location > 1)
		{
			return error(ERR_INVALIDARG);
		}

		const auto building_info = std::make_unique<database::players::building_info_t>();

		const auto do_list = [&](nlohmann::json& data, const std::uint32_t type)
		{
			if (!data.is_array())
			{
				return;
			}

			for (auto i = 0ull; i < data.size(); i++)
			{
				auto& column_j = data[i]["column"];
				auto& row_j = data[i]["row"];
				auto& current_life_j = data[i]["current_life"];

				if (!column_j.is_number_unsigned() || !row_j.is_number_unsigned() || !current_life_j.is_number_unsigned())
				{
					continue;
				}

				const auto col = column_j.get<std::uint32_t>();
				const auto row = row_j.get<std::uint32_t>();
				const auto current_life = current_life_j.get<std::uint16_t>();

				building_info->cells[row][col].edges[type].life = current_life;
			}
		};

		user->current_player->get_building_info(*building_info, map_location);

		do_list(data["center_life_list"], database::players::edge_type_upper);
		do_list(data["upper_life_list"], database::players::edge_type_upper);
		do_list(data["left_life_list"], database::players::edge_type_left);

		user->current_player->set_building_info(*building_info, map_location);

		auto& base_resource_j = data["base_resource"];
		if (base_resource_j.is_array() && base_resource_j.size())
		{
			const auto base_resources = std::make_unique<database::players::base_resources_t>();
			user->current_player->get_base_resources(*base_resources);
			base_resources->parse_base(base_resource_j[0]);
			user->current_player->set_base_resources(*base_resources);
		}

		// TODO: farming_update_list

		return result;
	}

	std::uint32_t cmd_building_remove::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
