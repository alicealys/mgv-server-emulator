#include <std_include.hpp>

#include "cmd_present_box_get_list.hpp"

// not implemented
namespace emulator::ssd
{
	glz::json cmd_present_box_get_list::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
