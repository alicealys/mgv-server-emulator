#include <std_include.hpp>

#include "cmd_deploy_supply_item.hpp"

// not implemented
namespace emulator::ssd
{
	json::value cmd_deploy_supply_item::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
