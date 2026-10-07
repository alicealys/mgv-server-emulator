#include <std_include.hpp>

#include "cmd_event_get_around_ranking.hpp"

namespace emulator::ssd
{
	json::value cmd_event_get_around_ranking::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		param_t param{};
		if (!json::read(param, data))
		{
			return error(ERR_INVALIDARG);
		}

		result["my_current_event_point"] = 0;
		result["my_disp_rank"] = 0;
		result["my_rank"] = 0;
		result["my_total_event_point"] = 0;
		result["ranking"] = json::array();
		result["total_number"] = 0;
		result["result"] = "ERR_NOTIMPLEMENTED";

		return result;
	}

	std::uint32_t cmd_event_get_around_ranking::flags()
	{
		return CMD_NEEDS_USER;
	}
}
