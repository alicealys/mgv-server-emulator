#include <std_include.hpp>

#include "cmd_base_resource_draw_water.hpp"

// not implemented
namespace emulator::ssd
{
	json::value cmd_base_resource_draw_water::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
