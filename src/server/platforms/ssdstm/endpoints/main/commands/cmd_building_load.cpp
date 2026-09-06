#include <std_include.hpp>

#include "cmd_building_load.hpp"

// not implemented
namespace emulator::ssd
{
	nlohmann::json cmd_building_load::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
