#include <std_include.hpp>

#include "cmd_wicked_report.hpp"

// not implemented
namespace emulator::ssd
{
	nlohmann::json cmd_wicked_report::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
