#include <std_include.hpp>

#include "cmd_error_info_send.hpp"

namespace emulator::ssd
{
	json::value cmd_error_info_send::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;
        
		param_t param{};
		if (!json::read(param, data))
		{
			return error(ERR_INVALIDARG);
		}
		
		return result;
	}

	std::uint32_t cmd_error_info_send::flags()
	{
		return CMD_NEEDS_USER;
	}
}
