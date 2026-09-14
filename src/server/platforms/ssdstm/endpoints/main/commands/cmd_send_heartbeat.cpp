#include <std_include.hpp>

#include "cmd_send_heartbeat.hpp"

// not implemented
namespace emulator::ssd
{
	json::value cmd_send_heartbeat::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
