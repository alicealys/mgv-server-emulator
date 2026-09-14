#include <std_include.hpp>

#include "cmd_steam_shop_get_item_list.hpp"

// not implemented
namespace emulator::ssd
{
	glz::json cmd_steam_shop_get_item_list::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
