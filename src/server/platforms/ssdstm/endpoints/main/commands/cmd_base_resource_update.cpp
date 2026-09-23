#include <std_include.hpp>

#include "cmd_base_resource_update.hpp"

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
			auto crew_member_list = std::make_unique<database::players::crew_member_list_t>();
			user->current_player->get_crew_member_list(*crew_member_list);

			if (crew_update_list_j.is_array())
			{
				crew_member_list->parse_update(crew_update_list_j);
			}

			if (crew_died_list_j.is_array())
			{
				for (auto i = 0ull; i < crew_died_list_j.size(); i++)
				{
					std::uint32_t unique_id{};
					if (!json::read(unique_id, crew_died_list_j[i]))
					{
						continue;
					}

					const auto member = crew_member_list->find_member(unique_id);
					std::memset(member, 0, sizeof(database::players::crew_member_list_t));
				}
			}

			user->current_player->set_crew_member_list(*crew_member_list);
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
