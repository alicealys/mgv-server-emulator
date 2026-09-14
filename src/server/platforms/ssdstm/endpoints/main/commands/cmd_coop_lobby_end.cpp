#include <std_include.hpp>

#include "cmd_coop_lobby_end.hpp"

// not implemented
namespace emulator::ssd
{
	glz::json cmd_coop_lobby_end::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
