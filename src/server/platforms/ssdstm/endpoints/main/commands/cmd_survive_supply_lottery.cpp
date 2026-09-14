#include <std_include.hpp>

#include "cmd_survive_supply_lottery.hpp"

// not implemented
namespace emulator::ssd
{
	json::value cmd_survive_supply_lottery::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
