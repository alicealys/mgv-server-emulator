#include <std_include.hpp>

#include "cmd_get_craft_param.hpp"

// not implemented
namespace emulator::ssd
{
	json::value cmd_get_craft_param::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
