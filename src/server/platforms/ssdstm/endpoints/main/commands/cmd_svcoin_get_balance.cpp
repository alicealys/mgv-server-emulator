#include <std_include.hpp>

#include "cmd_svcoin_get_balance.hpp"

// not implemented
namespace emulator::ssd
{
	json::value cmd_svcoin_get_balance::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
