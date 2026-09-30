#include <std_include.hpp>

#include "cmd_purchase_get_history.hpp"

#include "database/models/shop_purchases.hpp"

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

		const auto history = database::shop_purchases::get_history(user->get_user_id(), param.start, param.num);
		for (auto i = 0ull; i < history.size(); i++)
		{
			history[i].to_json(result["list"][i]);
		}

		result["history_num"] = database::shop_purchases::get_history_size(user->get_user_id());

		return result;
	}

	std::uint32_t cmd_purchase_get_history::flags()
	{
		return CMD_NEEDS_USER;
	}
}
