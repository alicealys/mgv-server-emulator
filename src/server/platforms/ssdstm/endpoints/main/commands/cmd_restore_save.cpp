#include <std_include.hpp>

#include "cmd_restore_save.hpp"

namespace emulator::ssd
{
	glz::json cmd_restore_save::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;

		static const auto allow_restore = config::get().allow_save_restore;
		if (!allow_restore)
		{
			return error(ERR_PERMISSION_DENIED);
		}

		[[ maybe_unused ]] auto& avatar_j = data["avatar"];
		[[ maybe_unused ]] auto& base_resources_j = data["base_resource"];
		[[ maybe_unused ]] auto& building_info_j = data["building"];
		[[ maybe_unused ]] auto& crew_j = data["crew"];
		[[ maybe_unused ]] auto& deploy_team_j = data["deploy_team"];
		[[ maybe_unused ]] auto& mission_progress_j = data["mission_progress"];
		[[ maybe_unused ]] auto& player_list_j = data["player_list"];

		if (avatar_j.is_object())
		{
			database::players::avatar_t avatar{};
			avatar.parse(avatar_j["avatar"]);
			user->current_player->set_avatar(avatar);
		}

		if (base_resources_j.is_object())
		{
			auto& base_resource_j = base_resources_j["base_resource"];
			auto& base_resource_count_j = base_resources_j["base_resource_count"];
			auto& animal_list_j = base_resources_j["animal_list"];

			const auto base_resources = std::make_unique<database::players::base_resources_t>();
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
		}

		if (building_info_j.is_array())
		{
			for (auto i = 0u; i < 2u; i++)
			{
				const auto building_info = std::make_unique<database::players::building_info_t>();
				if (!building_info_j[i].is_object())
				{
					continue;
				}

				building_info->parse(building_info_j[i]["cell_info"], false);
				user->current_player->set_building_info(*building_info, i);
			}
		}

		if (player_list_j.is_object())
		{
			auto& player_list_arr_j = player_list_j["player_list"];
			if (player_list_arr_j.is_array() && player_list_arr_j.size())
			{
				auto& nameplate_j = player_list_arr_j[0]["name_plate"];
				if (nameplate_j.is_uint64())
				{
					const auto nameplate = nameplate_j.as<std::uint16_t>();
					user->current_player->set_nameplate(nameplate);
				}
			}
		}

		if (crew_j.is_object())
		{
			auto& crew_level_j = crew_j["group_level"];
			if (crew_level_j.is_object())
			{
				auto crew_levels = std::make_unique<database::players::crew_levels_t>();
				crew_levels->parse(crew_level_j);
				user->current_player->set_crew_levels(*crew_levels);
			}

			auto& crew_list_j = crew_j["crew_list"];
			if (crew_list_j.is_array())
			{
				auto crew_member_list = std::make_unique<database::players::crew_member_list_t>();
				crew_member_list->parse(crew_list_j);
				user->current_player->set_crew_member_list(*crew_member_list);
			}
		}

		// TODO: deploy team
		// if (deploy_team_j.is_object())
		// {
		// 
		// }

		if (mission_progress_j.is_object())
		{
			auto& current_mission_info_j = mission_progress_j["current_mission_info"];
			auto& gimmick_save_info_afghan_j = mission_progress_j["gimmick_save_info_afghan"];
			auto& gimmick_save_info_africa_j = mission_progress_j["gimmick_save_info_africa"];
			auto& gimmick_timer_info_j = mission_progress_j["gimmick_timer_info"];
			auto& inventory_player_info_j = mission_progress_j["inventory_player_info"];
			auto& inventory_user_info_j = mission_progress_j["inventory_user_info"];
			auto& load_out_list_j = mission_progress_j["load_out_list"];
			auto& mission_record_info_list_j = mission_progress_j["mission_record_info_list"];
			auto& nonstackable_list_j = mission_progress_j["nonstackable_list"];
			auto& resource_list_j = mission_progress_j["resource_list"];
			auto& stackable_list_j = mission_progress_j["stackable_list"];
			auto& story_unlock_info_j = mission_progress_j["story_unlock_info"];
			auto& user_flag_j = mission_progress_j["user_flag"];

			auto gimmick_info = std::make_unique<database::players::gimmick_info_t>();
			if (gimmick_save_info_afghan_j.is_object())
			{
				auto gimmick_data = std::make_unique<database::players::gimmick_save_data_t>();
				if (gimmick_data->parse(gimmick_save_info_afghan_j) && gimmick_info->resource_afghan.parse(gimmick_save_info_afghan_j))
				{
					user->current_player->set_gimmick_save_data(*gimmick_data, 0);
				}
			}

			if (gimmick_save_info_africa_j.is_object())
			{
				auto gimmick_data = std::make_unique<database::players::gimmick_save_data_t>();
				if (gimmick_data->parse(gimmick_save_info_africa_j) && gimmick_info->resource_africa.parse(gimmick_save_info_africa_j))
				{
					user->current_player->set_gimmick_save_data(*gimmick_data, 1);
				}
			}

			if (gimmick_timer_info_j.is_object())
			{
				gimmick_info->timer.parse(gimmick_timer_info_j);
			}

			user->current_player->set_gimmick_info(*gimmick_info);

			if (inventory_player_info_j.is_object())
			{
				auto inventory = std::make_unique<database::players::player_inventory_t>();
				std::uint16_t nameplate{};
				if (inventory->parse(inventory_player_info_j, nameplate))
				{
					user->current_player->set_inventory(*inventory);
				}
			}

			if (inventory_user_info_j.is_object())
			{
				auto inventory = std::make_unique<database::users::user_inventory_t>();
				if (inventory->parse(inventory_user_info_j))
				{
					user->set_inventory(*inventory);
				}
			}

			if (load_out_list_j.is_array())
			{
				auto loadouts = std::make_unique<database::players::loadout_list_t>();
				auto loadout = std::make_unique<database::players::loadout_t>();

				for (auto i = 0ull; i < load_out_list_j.size(); i++)
				{
					std::uint32_t index{};
					if (!loadout->parse(load_out_list_j[i], index))
					{
						continue;
					}

					if (index < ARRAYSIZE(loadouts->list))
					{
						std::memcpy(&loadouts->list[i], loadout.get(), sizeof(database::players::loadout_t));
					}
				}

				user->current_player->set_loadout_list(*loadouts);
			}

			if (mission_record_info_list_j.is_array())
			{
				auto mission_record_list = std::make_unique<database::players::mission_record_list_t>();
				if (mission_record_list->parse(mission_record_info_list_j))
				{
					user->current_player->set_mission_record_list(*mission_record_list);
				}
			}

			if (nonstackable_list_j.is_array())
			{
				auto nonstackable_list = std::make_unique<database::players::nonstackable_item_list_t>();
				if (nonstackable_list->parse(nonstackable_list_j))
				{
					user->current_player->set_nonstackable_item_list(*nonstackable_list);
				}
			}

			if (resource_list_j.is_array())
			{
				auto resource_list = std::make_unique<database::players::inventory_resource_list_t>();
				if (resource_list->parse(resource_list_j))
				{
					user->current_player->set_inventory_resource_list(*resource_list);
				}
			}

			if (stackable_list_j.is_array())
			{
				auto stackable_list = std::make_unique<database::players::stackable_item_list_t>();
				if (stackable_list->parse(stackable_list_j))
				{
					user->current_player->set_stackable_item_list(*stackable_list);
				}
			}

			if (story_unlock_info_j.is_object())
			{
				auto info = std::make_unique<database::players::story_unlock_info_t>();
				if (info->parse(story_unlock_info_j))
				{
					user->current_player->set_story_unlock_info(*info);
				}

				auto& unlock_list_j = story_unlock_info_j["map_unlock_list"];
				if (unlock_list_j.is_array())
				{
					for (auto i = 0ull; i < unlock_list_j.size(); i++)
					{
						auto& location_index_j = unlock_list_j[i]["location_index"];
						if (!location_index_j.is_uint64())
						{
							continue;
						}

						const auto location_index = location_index_j.as<std::uint32_t>();

						auto unlock_list = std::make_unique<database::players::map_unlock_list_t>();
						unlock_list->parse_diff(unlock_list_j[i]["map_unlock"]);

						switch (location_index)
						{
						case 0:
							user->current_player->set_map_unlock_list_afghan(*unlock_list);
							break;
						case 1:
							user->current_player->set_map_unlock_list_africa(*unlock_list);
							break;
						}
					}
				}

				if (current_mission_info_j.is_object())
				{
					auto mission_info = std::make_unique<database::players::mission_info_t>();
					if (mission_info->parse(current_mission_info_j))
					{
						user->current_player->set_mission_info(*mission_info);
					}
				}
			}

			if (user_flag_j.is_uint64())
			{
				const auto user_flag = user_flag_j.as<std::uint32_t>();
				user->set_user_flag(user_flag);
			}

			database::users::reset_current_player(user->get_user_id());
		}

		return result;
	}

	std::uint32_t cmd_restore_save::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
