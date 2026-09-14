#include <std_include.hpp>

#include "cmd_event_shop_get_list.hpp"

// not implemented
namespace emulator::ssd
{
	json::value cmd_event_shop_get_list::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
