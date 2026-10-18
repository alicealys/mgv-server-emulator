#include <std_include.hpp>

#include "cmd_building_remove.hpp"

namespace emulator::ssd
{
	json::value cmd_building_remove::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		const auto building_info = std::make_unique<database::players::building_info_t>();
		user->current_player->get_building_info(*building_info);

		const auto do_list = [&](json::value& data, const std::uint32_t type)
		{
			if (!data.is_array())
			{
				return true;
			}

			for (auto i = 0ull; i < data.size(); i++)
			{
				auto& column_j = data[i]["column"];
				auto& row_j = data[i]["row"];
				auto& current_life_j = data[i]["current_life"];

				if (!column_j.is_uint64() || !row_j.is_uint64() || !current_life_j.is_uint64())
				{
					return false;
				}

				const auto col = column_j.as<std::uint32_t>();
				const auto row = row_j.as<std::uint32_t>();
				const auto current_life = current_life_j.as<std::uint16_t>();

				auto& edge = building_info->cells[row][col].edges[type];
				if (edge.production_index == 0)
				{
					return false;
				}

				if (current_life == 0)
				{
					std::memset(&edge, 0, sizeof(database::players::building_info_t::cell_edge_t));
				}
				else
				{
					edge.life = current_life;
				}
			}

			return true;
		};

		if (!do_list(data["center_life_list"], database::players::edge_type_center) ||
			!do_list(data["upper_life_list"], database::players::edge_type_upper) ||
			!do_list(data["left_life_list"], database::players::edge_type_left))
		{
			return error(ERR_DATABASE);
		}

		auto& base_resource_j = data["base_resource"];
		if (base_resource_j.is_array() && base_resource_j.size())
		{
			const auto base_resources = std::make_unique<database::players::base_resources_t>();
			user->current_player->get_base_resources(*base_resources);
			base_resources->parse_base(base_resource_j[0]);
			user->current_player->set_base_resources(*base_resources);
		}

		building_info->parse_farming(data["farming_update_list"]);

		user->current_player->set_building_info(*building_info);

		return result;
	}

	std::uint32_t cmd_building_remove::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
