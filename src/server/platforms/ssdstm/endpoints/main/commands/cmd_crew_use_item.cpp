#include <std_include.hpp>

#include "cmd_crew_use_item.hpp"

// not implemented
namespace emulator::ssd
{
	glz::json cmd_crew_use_item::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
