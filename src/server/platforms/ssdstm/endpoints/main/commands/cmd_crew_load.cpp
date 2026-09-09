#include <std_include.hpp>

#include "cmd_crew_load.hpp"

// not implemented
namespace emulator::ssd
{
	nlohmann::json cmd_crew_load::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;

		// TODO

		result["group_level"]["base_defense"] = 0;
		result["group_level"]["combat_deploy"] = 0;
		result["group_level"]["develop"] = 0;
		result["group_level"]["food"] = 0;
		result["group_level"]["medic"] = 0;
		result["group_level"]["plant"] = 0;

        return result;
	}
}
