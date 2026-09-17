#include <std_include.hpp>

#include "cmd_error_info_send.hpp"

struct param_t
{
	std::uint32_t action;
	std::uint32_t error;
	std::uint32_t info;
	std::string message;
};

namespace emulator::ssd
{
	json::value cmd_error_info_send::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;
        
		param_t param{};
		if (json::read(param, data))
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
