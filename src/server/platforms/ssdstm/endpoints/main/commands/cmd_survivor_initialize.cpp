#include <std_include.hpp>

#include "cmd_survivor_initialize.hpp"

// not implemented
namespace emulator::ssd
{
	json::value cmd_survivor_initialize::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
