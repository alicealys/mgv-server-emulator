#include <std_include.hpp>

#include "cmd_dlc_set_flag.hpp"

namespace emulator::ssd
{
	nlohmann::json cmd_dlc_set_flag::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;

		auto& flag_j = data["flag"];

		if (!flag_j.is_number_unsigned())
		{
			return error(ERR_INVALIDARG);
		}

		const auto flag = user->get_dlc_flag() | flag_j.get<std::uint32_t>();
		user->set_dlc_flag(flag);

		return result;
	}

	std::uint32_t cmd_dlc_set_flag::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
