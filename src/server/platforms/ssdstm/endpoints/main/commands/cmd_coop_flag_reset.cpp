#include <std_include.hpp>

#include "cmd_coop_flag_reset.hpp"

// not implemented
namespace emulator::ssd
{
	json::value cmd_coop_flag_reset::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
