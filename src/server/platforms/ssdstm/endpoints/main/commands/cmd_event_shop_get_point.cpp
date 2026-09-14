#include <std_include.hpp>

#include "cmd_event_shop_get_point.hpp"

// not implemented
namespace emulator::ssd
{
	glz::json cmd_event_shop_get_point::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
