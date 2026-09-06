#include <std_include.hpp>

#include "cmd_statistics_matching_time.hpp"

// not implemented
namespace emulator::ssd
{
	nlohmann::json cmd_statistics_matching_time::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
