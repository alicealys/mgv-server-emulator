#include <std_include.hpp>

#include "cmd_purchase_shop_get_item_list.hpp"

namespace emulator::ssd
{
	json::value cmd_purchase_shop_get_item_list::execute(json::value& data, const std::optional<database::users::user>& user)
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
		// item.param5
		// item.num
		// limit_count
		// current_count
		// price
		// flag
		result["item_list"] = json::array();
		result["next_date"] = 0;
		result["result"] = "ERR_NOTIMPLEMENTED";

		return result;
	}

	std::uint32_t cmd_purchase_shop_get_item_list::flags()
	{
		return CMD_NEEDS_USER;
	}
}
