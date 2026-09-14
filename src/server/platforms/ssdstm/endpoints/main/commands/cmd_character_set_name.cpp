#include <std_include.hpp>

#include "cmd_character_set_name.hpp"

// not implemented
namespace emulator::ssd
{
	glz::json cmd_character_set_name::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
