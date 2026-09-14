#include <std_include.hpp>

#include "cmd_dlc_set_flag.hpp"

namespace emulator::ssd
{
	json::value cmd_dlc_set_flag::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		auto& flag_j = data["flag"];

		if (!flag_j.is_uint64())
		{
			return error(ERR_INVALIDARG);
		}

		const auto flag = user->get_dlc_flag() | flag_j.as<std::uint32_t>();
		user->set_dlc_flag(flag);

		return result;
	}

	std::uint32_t cmd_dlc_set_flag::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
