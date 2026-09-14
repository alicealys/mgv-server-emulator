#include <std_include.hpp>

#include "cmd_get_friend_loadout.hpp"

// not implemented
namespace emulator::ssd
{
	json::value cmd_get_friend_loadout::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
