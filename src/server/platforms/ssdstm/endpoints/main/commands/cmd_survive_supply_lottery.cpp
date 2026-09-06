#include <std_include.hpp>

#include "cmd_survive_supply_lottery.hpp"

// not implemented
namespace emulator::ssd
{
	nlohmann::json cmd_survive_supply_lottery::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
