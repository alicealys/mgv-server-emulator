#include <std_include.hpp>

#include "cmd_mission_abort.hpp"

// not implemented
namespace emulator::ssd
{
	nlohmann::json cmd_mission_abort::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
