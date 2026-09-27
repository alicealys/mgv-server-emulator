#include <std_include.hpp>

#include "cmd_svcoin_get_balance.hpp"

namespace emulator::ssd
{
	json::value cmd_svcoin_get_balance::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		result["balance"] = user->get_sv_coin();

		return result;
	}

	std::uint32_t cmd_svcoin_get_balance::flags()
	{
		return CMD_NEEDS_USER;
	}
}
