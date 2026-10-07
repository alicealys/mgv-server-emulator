#include <std_include.hpp>

#include "cmd_present_box_get_list.hpp"

#include "database/models/present_box.hpp"

namespace emulator::ssd
{
	json::value cmd_present_box_get_list::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		param_t param{};
		if (!json::read(param, data))
		{
			return error(ERR_INVALIDARG);
		}

		result["is_organized"] = 0;
		result["present_list"] = json::array();

		const auto now = std::chrono::system_clock::now();
		const auto list = database::present_box::get_all_items(user->get_user_id());

		auto count = 0u;
		for (auto i = 0ull; i < list.size(); i++)
		{
			if (list[i].get_expire_date() < now.time_since_epoch())
			{
				continue;
			}

			list[i].to_json(result["present_list"][count++]);
		}

		result["present_num"] = count;

		return result;
	}

	std::uint32_t cmd_present_box_get_list::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
