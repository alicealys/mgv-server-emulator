#include <std_include.hpp>

#include "cmd_reqauth_sessionsvr.hpp"

namespace emulator::ssd
{
	json::value cmd_reqauth_sessionsvr::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		param_t param{};
		if (json::read(param, data))
		{
			return error(ERR_INVALIDARG);
		}

		result["first_login"] = "NotImplement";
		result["result"] = "ERR_NOTIMPLEMENTED";

		return result;
	}

	std::uint32_t cmd_reqauth_sessionsvr::flags()
	{
		return CMD_NEEDS_USER;
	}
}
