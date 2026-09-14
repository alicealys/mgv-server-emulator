#include <std_include.hpp>

#include "cmd_steam_shop_open.hpp"

// not implemented
namespace emulator::ssd
{
	json::value cmd_steam_shop_open::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
