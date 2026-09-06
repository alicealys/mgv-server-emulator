#include <std_include.hpp>

#include "cmd_craft_check_crafted.hpp"

// not implemented
namespace emulator::ssd
{
	nlohmann::json cmd_craft_check_crafted::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
