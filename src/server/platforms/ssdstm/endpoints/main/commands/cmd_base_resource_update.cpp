#include <std_include.hpp>

#include "cmd_base_resource_update.hpp"

// not implemented
namespace emulator::ssd
{
	json::value cmd_base_resource_update::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
