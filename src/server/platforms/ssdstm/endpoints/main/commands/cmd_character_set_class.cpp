#include <std_include.hpp>

#include "cmd_character_set_class.hpp"

// not implemented
namespace emulator::ssd
{
	glz::json cmd_character_set_class::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
