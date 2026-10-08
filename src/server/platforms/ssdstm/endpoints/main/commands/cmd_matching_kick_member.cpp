#include <std_include.hpp>

#include "cmd_matching_kick_member.hpp"

#include "database/models/matching.hpp"

namespace emulator::ssd
{
	json::value cmd_matching_kick_member::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		param_t param{};
		if (!json::read(param, data))
		{
			return error(ERR_INVALIDARG);
		}

		return result;
	}

	std::uint32_t cmd_matching_kick_member::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
