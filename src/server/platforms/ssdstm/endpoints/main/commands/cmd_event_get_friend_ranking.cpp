#include <std_include.hpp>

#include "cmd_event_get_friend_ranking.hpp"

namespace emulator::ssd
{
	json::value cmd_event_get_friend_ranking::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		param_t param{};
		if (!json::read(param, data))
		{
			return error(ERR_INVALIDARG);
		}

		result["my_disp_rank"] = 0;
		result["my_rank"] = 0;
		result["ranking"] = json::array();
		result["result"] = "ERR_NOTIMPLEMENTED";

		return result;
	}

	std::uint32_t cmd_event_get_friend_ranking::flags()
	{
		return CMD_NEEDS_USER;
	}
}
