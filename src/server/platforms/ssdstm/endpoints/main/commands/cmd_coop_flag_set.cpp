#include <std_include.hpp>

#include "cmd_coop_flag_set.hpp"

// not implemented
namespace emulator::ssd
{
	nlohmann::json cmd_coop_flag_set::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
