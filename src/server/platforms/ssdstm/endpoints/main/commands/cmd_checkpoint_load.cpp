#include <std_include.hpp>

#include "cmd_checkpoint_load.hpp"

// not implemented
namespace emulator::ssd
{
	json::value cmd_checkpoint_load::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
