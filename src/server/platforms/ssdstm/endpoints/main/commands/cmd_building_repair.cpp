#include <std_include.hpp>

#include "cmd_building_repair.hpp"

// not implemented
namespace emulator::ssd
{
	nlohmann::json cmd_building_repair::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
