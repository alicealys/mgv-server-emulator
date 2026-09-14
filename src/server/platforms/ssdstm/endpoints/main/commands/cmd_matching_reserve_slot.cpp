#include <std_include.hpp>

#include "cmd_matching_reserve_slot.hpp"

// not implemented
namespace emulator::ssd
{
	json::value cmd_matching_reserve_slot::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
