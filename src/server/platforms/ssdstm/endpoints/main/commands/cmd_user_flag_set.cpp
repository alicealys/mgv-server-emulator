#include <std_include.hpp>

#include "cmd_user_flag_set.hpp"

namespace emulator::ssd
{
	nlohmann::json cmd_user_flag_set::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;

		auto& flag_j = data["flag"];

		if (!flag_j.is_number_unsigned())
		{
			return error(ERR_INVALIDARG);
		}

		const auto flag = user->get_user_flag() | flag_j.get<std::uint32_t>();
		user->set_user_flag(flag);

        return result;
	}

	std::uint32_t cmd_user_flag_set::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
