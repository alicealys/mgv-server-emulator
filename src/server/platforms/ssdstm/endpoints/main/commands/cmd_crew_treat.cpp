#include <std_include.hpp>

#include "cmd_crew_treat.hpp"

// not implemented
namespace emulator::ssd
{
	json::value cmd_crew_treat::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
