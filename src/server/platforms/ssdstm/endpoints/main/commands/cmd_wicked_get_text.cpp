#include <std_include.hpp>

#include "cmd_wicked_get_text.hpp"

// not implemented
namespace emulator::ssd
{
	glz::json cmd_wicked_get_text::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
