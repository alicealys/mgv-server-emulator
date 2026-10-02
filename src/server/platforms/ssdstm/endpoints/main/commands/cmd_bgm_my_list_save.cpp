#include <std_include.hpp>

#include "cmd_bgm_my_list_save.hpp"

namespace emulator::ssd
{
	json::value cmd_bgm_my_list_save::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		const auto user_inventory = std::make_unique<database::users::user_inventory_t>();
		user->get_inventory(*user_inventory);

		if (!user_inventory->bgm_settings.parse(data))
		{
			return error(ERR_INVALIDARG);
		}

		user->set_inventory(*user_inventory);

		return result;
	}

	std::uint32_t cmd_bgm_my_list_save::flags()
	{
		return CMD_NEEDS_USER;
	}
}
