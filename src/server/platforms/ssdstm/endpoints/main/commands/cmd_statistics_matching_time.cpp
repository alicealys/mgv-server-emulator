#include <std_include.hpp>

#include "cmd_statistics_matching_time.hpp"

// not implemented
namespace emulator::ssd
{
	json::value cmd_statistics_matching_time::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
