#include <std_include.hpp>

#include "cmd_event_get_friend_ranking.hpp"

// not implemented
namespace emulator::ssd
{
	glz::json cmd_event_get_friend_ranking::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
