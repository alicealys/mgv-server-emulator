#include <std_include.hpp>

#include "cmd_event_get_ranking.hpp"

// not implemented
namespace emulator::ssd
{
	nlohmann::json cmd_event_get_ranking::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
