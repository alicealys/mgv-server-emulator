#include <std_include.hpp>

#include "cmd_craft_open_perk.hpp"

// not implemented
namespace emulator::ssd
{
	json::value cmd_craft_open_perk::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
