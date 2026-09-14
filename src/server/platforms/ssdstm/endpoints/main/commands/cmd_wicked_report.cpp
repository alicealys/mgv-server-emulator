#include <std_include.hpp>

#include "cmd_wicked_report.hpp"

// not implemented
namespace emulator::ssd
{
	json::value cmd_wicked_report::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
