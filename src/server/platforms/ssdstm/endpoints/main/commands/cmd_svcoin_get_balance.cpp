#include <std_include.hpp>

#include "cmd_svcoin_get_balance.hpp"

// not implemented
namespace emulator::ssd
{
	nlohmann::json cmd_svcoin_get_balance::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
