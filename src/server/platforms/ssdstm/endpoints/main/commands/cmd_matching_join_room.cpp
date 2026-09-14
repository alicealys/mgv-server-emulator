#include <std_include.hpp>

#include "cmd_matching_join_room.hpp"

// not implemented
namespace emulator::ssd
{
	glz::json cmd_matching_join_room::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
