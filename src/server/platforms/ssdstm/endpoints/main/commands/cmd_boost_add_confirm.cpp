#include <std_include.hpp>

#include "cmd_boost_add_confirm.hpp"

// not implemented
namespace emulator::ssd
{
	glz::json cmd_boost_add_confirm::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
