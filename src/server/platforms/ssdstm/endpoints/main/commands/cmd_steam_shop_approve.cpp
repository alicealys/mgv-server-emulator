#include <std_include.hpp>

#include "cmd_steam_shop_approve.hpp"

// not implemented
namespace emulator::ssd
{
	glz::json cmd_steam_shop_approve::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
