#include <std_include.hpp>

#include "cmd_get_craft_param.hpp"

// not implemented
namespace emulator::ssd
{
	glz::json cmd_get_craft_param::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
