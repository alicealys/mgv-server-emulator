#include <std_include.hpp>

#include "cmd_base_resource_update.hpp"

#include "database/models/crew_members.hpp"

namespace emulator::ssd
{
	json::value cmd_base_resource_update::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		auto& base_resource_j = data["base_resource"];
		auto& base_resource_count_j = data["base_resource_count"];
		auto& animal_list_j = data["animal_list"];
		auto& crew_update_list_j = data["crew_update_list"];
		auto& crew_died_list_j = data["crew_died_list"];
		auto& farming_update_list_j = data["farming_update_list"];

		const auto base_resources = std::make_unique<database::players::base_resources_t>();
		user->current_player->get_base_resources(*base_resources);

		if (base_resource_count_j.is_array() && base_resource_count_j.size())
		{
			base_resources->parse_counts(base_resource_count_j);
		}

		if (base_resource_j.is_array() && base_resource_j.size())
		{
			base_resources->parse_base(base_resource_j);
		}

		if (animal_list_j.is_array() && animal_list_j.size())
		{
			base_resources->parse_animals(animal_list_j);
		}

		user->current_player->set_base_resources(*base_resources);

		if (crew_update_list_j.is_array() || crew_died_list_j.is_array())
		{
			if (crew_update_list_j.is_array())
			{
				for (auto i = 0ull; i < crew_update_list_j.size(); i++)
				{
					database::crew_members::member_params_t update_params{};
					if (!update_params.parse_update(crew_update_list_j[i]))
					{
						continue;
					}

					const auto member = database::crew_members::find(user->current_player->get_player_id(), update_params.unique_index);
					if (!member.has_value())
					{
						continue;
					}

					member->update(update_params);
				}
			}

			if (crew_died_list_j.is_array())
			{
				for (auto i = 0ull; i < crew_died_list_j.size(); i++)
				{
					std::uint64_t unique_id{};
					if (!json::read(unique_id, crew_died_list_j[i]))
					{
						continue;
					}

					database::crew_members::remove(user->current_player->get_player_id(), unique_id);
				}
			}
		}

		if (farming_update_list_j.is_array())
		{
			const auto building_info = std::make_unique<database::players::building_info_t>();
			user->current_player->get_building_info(*building_info);
			building_info->parse_farming(farming_update_list_j);
			user->current_player->set_building_info(*building_info);
		}

		return result;
	}

	std::uint32_t cmd_base_resource_update::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
