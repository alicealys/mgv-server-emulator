#include <std_include.hpp>

#include "cmd_purchase_get_history.hpp"

namespace emulator::ssd
{
	json::value cmd_purchase_get_history::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		param_t param{};
		if (!json::read(param, data))
		{
			return error(ERR_INVALIDARG);
		}

		// date
		// item_type
		// lang_id
		// item.category
		// item.code
		// item.param1
		// item.param2
		// item.param3
		// item.param4
		// item.param5
		// item.num
		// event_type
		// coin_quantity
		// item_quantity
		// expire_date
		// remaining_coin
		result["list"] = json::array();
		result["history_num"] = 0;
		result["result"] = "ERR_NOTIMPLEMENTED";

		return result;
	}

	std::uint32_t cmd_purchase_get_history::flags()
	{
		return CMD_NEEDS_USER;
	}
}
