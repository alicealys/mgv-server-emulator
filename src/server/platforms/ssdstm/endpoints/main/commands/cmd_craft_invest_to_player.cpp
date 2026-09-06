#include <std_include.hpp>

#include "cmd_craft_invest_to_player.hpp"

// not implemented
namespace emulator::ssd
{
	nlohmann::json cmd_craft_invest_to_player::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
