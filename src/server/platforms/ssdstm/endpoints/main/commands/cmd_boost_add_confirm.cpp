#include <std_include.hpp>

#include "cmd_boost_add_confirm.hpp"

// not implemented
namespace emulator::ssd
{
	nlohmann::json cmd_boost_add_confirm::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
