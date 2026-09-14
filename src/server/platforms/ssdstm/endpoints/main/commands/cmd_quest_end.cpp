#include <std_include.hpp>

#include "cmd_quest_end.hpp"

// not implemented
namespace emulator::ssd
{
	glz::json cmd_quest_end::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
