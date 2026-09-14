#include <std_include.hpp>

#include "cmd_get_friend_loadout.hpp"

// not implemented
namespace emulator::ssd
{
	glz::json cmd_get_friend_loadout::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
