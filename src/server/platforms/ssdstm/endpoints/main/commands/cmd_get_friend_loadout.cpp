#include <std_include.hpp>

#include "cmd_get_friend_loadout.hpp"

// not implemented
namespace emulator::ssd
{
	nlohmann::json cmd_get_friend_loadout::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
