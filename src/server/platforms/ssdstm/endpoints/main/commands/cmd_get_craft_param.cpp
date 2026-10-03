#include <std_include.hpp>

#include "cmd_get_craft_param.hpp"

namespace emulator::ssd
{
	json::value cmd_get_craft_param::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		result["crew_param"] = game::parameters_table.ssd_crew_generator_table->dump();
		result["damage_param"] = game::parameters_table.ssd_damage_parameter->dump();
		result["equip_param"] = game::parameters_table.ssd_equip_parameters->dump();
		result["sbm_param"] = game::parameters_table.ssd_sbm_parameters->dump();
		result["weapon_param"] = game::parameters_table.ssd_weapon_parameters->dump();

		return result;
	}

	std::uint32_t cmd_get_craft_param::flags()
	{
		return CMD_NEEDS_USER;
	}
}
