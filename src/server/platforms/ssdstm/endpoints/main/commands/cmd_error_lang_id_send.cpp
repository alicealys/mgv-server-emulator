#include <std_include.hpp>

#include "cmd_error_lang_id_send.hpp"

namespace emulator::ssd
{
	json::value cmd_error_lang_id_send::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;
        
		std::uint32_t id{};
		if (!json::read(id, data["id"]))
		{
			return error(ERR_INVALIDARG);
		}
		
		return result;
	}

	std::uint32_t cmd_error_lang_id_send::flags()
	{
		return CMD_NEEDS_USER;
	}
}
