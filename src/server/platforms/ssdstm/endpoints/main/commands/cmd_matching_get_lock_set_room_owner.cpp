#include <std_include.hpp>

#include "cmd_matching_get_lock_set_room_owner.hpp"

// not implemented
namespace emulator::ssd
{
	glz::json cmd_matching_get_lock_set_room_owner::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
