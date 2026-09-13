#include <std_include.hpp>

#include "cmd_crew_add.hpp"

namespace emulator::ssd
{
	nlohmann::json cmd_crew_add::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;

		auto& add_crew_param = data["add_crew_param"];
		if (!add_crew_param.is_array())
		{
			return error(ERR_INVALIDARG);
		}

		auto crew_member_list = std::make_unique<database::players::crew_member_list_t>();
		user->current_player->get_crew_member_list(*crew_member_list);

		result["added_crew"] = nlohmann::json::array();
		auto count = 0u;

		for (auto i = 0ull; i < add_crew_param.size(); i++)
		{
			database::players::crew_member_t new_member{};
			if (!new_member.parse_add_param(add_crew_param[i]))
			{
				continue;
			}

			const auto iter = game::parameters_table.ssd_crew_generator_table->crew_member_types.find(new_member.unique_index);
			if (iter == game::parameters_table.ssd_crew_generator_table->crew_member_types.end())
			{
				continue;
			}

			new_member.unique_id = utils::cryptography::random::get_integer() % 1000000;
			new_member.face_id = iter->second->face;
			new_member.race_id = iter->second->race;
			new_member.body_id = iter->second->body;
			new_member.ability_accessory = 0;
			new_member.ability_animal = 0;
			new_member.ability_base_defense = iter->second->base_defense;
			new_member.ability_defense_unit = 0;
			new_member.ability_develop = iter->second->develop;
			new_member.ability_expedition = iter->second->combat_deploy;
			new_member.ability_food = iter->second->food;
			new_member.ability_gadget = 0;
			new_member.ability_medical = iter->second->medic;
			new_member.ability_plant = iter->second->farm;
			new_member.resistance_food_shortage = iter->second->hunger_resist;
			new_member.resistance_sleepless = iter->second->sleeplack_resist;
			new_member.resistance_water_shortage = iter->second->thirst_resist;
			new_member.sanity = 1000;
			new_member.max_life = iter->second->life;
			new_member.life = iter->second->life;
			new_member.initial_max_life = iter->second->life;
			new_member.current_group = 1;
			new_member.previous_group = 1;

			crew_member_list->push(new_member);
			new_member.to_json(result["added_crew"][count++]);
		}

		return result;
	}

	std::uint32_t cmd_crew_add::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
