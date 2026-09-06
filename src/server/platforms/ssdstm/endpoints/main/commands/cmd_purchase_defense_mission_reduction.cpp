#include <std_include.hpp>

#include "cmd_purchase_defense_mission_reduction.hpp"

// not implemented
namespace emulator::ssd
{
	nlohmann::json cmd_purchase_defense_mission_reduction::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
