#include <std_include.hpp>

#include "cmd_matching_alive_notice.hpp"

// not implemented
namespace emulator::ssd
{
	nlohmann::json cmd_matching_alive_notice::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
