#include <std_include.hpp>

#include "cmd_steam_shop_approve.hpp"

// not implemented
namespace emulator::ssd
{
	nlohmann::json cmd_steam_shop_approve::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
