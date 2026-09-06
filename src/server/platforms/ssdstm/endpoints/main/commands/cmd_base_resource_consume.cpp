#include <std_include.hpp>

#include "cmd_base_resource_consume.hpp"

// not implemented
namespace emulator::ssd
{
	nlohmann::json cmd_base_resource_consume::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
