#include <std_include.hpp>

#include "cmd_base_resource_provide.hpp"

// not implemented
namespace emulator::ssd
{
	glz::json cmd_base_resource_provide::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
