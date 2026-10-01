#include <std_include.hpp>

#include "cmd_steam_shop_open.hpp"
#include "cmd_steam_shop_get_item_list.hpp"

namespace emulator::ssd
{
	json::value cmd_steam_shop_open::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		param_t param{};
		if (!json::read(param, data))
		{
			return error(ERR_INVALIDARG);
		}

		if (param.steam_item_id >= cmd_steam_shop_get_item_list::items.size())
		{
			return error(ERR_NOT_FOUND);
		}

		const auto& item = cmd_steam_shop_get_item_list::items[param.steam_item_id];
		if (item.callback != nullptr)
		{
			item.callback(item.info, user.value());
		}

		result["orderid"] = 0;
		result["transid"] = 0;
		result["result"] = game::error_map[ERR_INVALIDARG];

		return result;
	}

	std::uint32_t cmd_steam_shop_open::flags()
	{
		return CMD_NEEDS_USER;
	}
}
