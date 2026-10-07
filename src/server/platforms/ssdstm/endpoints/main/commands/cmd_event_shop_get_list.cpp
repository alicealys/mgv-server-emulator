#include <std_include.hpp>

#include "cmd_event_shop_get_list.hpp"

// not implemented
namespace emulator::ssd
{
	json::value cmd_event_shop_get_list::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		// id
		// lang_id
		// item.category
		// item.code
		// item.param1
		// item.param2
		// item.param3
		// item.param4
		// item.num
		// limit_count
		// current_count
		// price
		// flag
		result["item_list"] = json::array();
		result["result"] = "ERR_NOTIMPLEMENTED";

		return result;
	}

	std::uint32_t cmd_event_shop_get_list::flags()
	{
		return CMD_NEEDS_USER;
	}
}
