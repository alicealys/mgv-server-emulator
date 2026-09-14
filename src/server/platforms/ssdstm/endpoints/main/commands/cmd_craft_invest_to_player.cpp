#include <std_include.hpp>

#include "cmd_craft_invest_to_player.hpp"

// not implemented
namespace emulator::ssd
{
	glz::json cmd_craft_invest_to_player::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
