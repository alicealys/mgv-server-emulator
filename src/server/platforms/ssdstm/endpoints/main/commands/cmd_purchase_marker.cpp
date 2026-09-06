#include <std_include.hpp>

#include "cmd_purchase_marker.hpp"

// not implemented
namespace emulator::ssd
{
	nlohmann::json cmd_purchase_marker::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
