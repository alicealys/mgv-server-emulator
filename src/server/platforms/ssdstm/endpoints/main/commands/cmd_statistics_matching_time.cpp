#include <std_include.hpp>

#include "cmd_statistics_matching_time.hpp"

namespace emulator::ssd
{
	json::value cmd_statistics_matching_time::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		param_t param{};
		if (!json::read(param, data))
		{
			return error(ERR_INVALIDARG);
		}

		return result;
	}

	std::uint32_t cmd_statistics_matching_time::flags()
	{
		return CMD_NEEDS_USER;
	}
}
