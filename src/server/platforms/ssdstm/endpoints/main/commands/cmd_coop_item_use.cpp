#include <std_include.hpp>

#include "cmd_coop_item_use.hpp"

// not implemented
namespace emulator::ssd
{
	glz::json cmd_coop_item_use::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
