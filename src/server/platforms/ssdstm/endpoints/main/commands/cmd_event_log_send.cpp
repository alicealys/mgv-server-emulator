#include <std_include.hpp>

#include "cmd_event_log_send.hpp"

// not implemented
namespace emulator::ssd
{
	json::value cmd_event_log_send::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
