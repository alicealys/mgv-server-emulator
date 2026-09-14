#include <std_include.hpp>

#include "cmd_coop_flag_set.hpp"

// not implemented
namespace emulator::ssd
{
	glz::json cmd_coop_flag_set::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
