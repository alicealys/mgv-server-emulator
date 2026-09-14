#include <std_include.hpp>

#include "cmd_event_get_info.hpp"

namespace emulator::ssd
{
	json::value cmd_event_get_info::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		// TODO
		result["event_list"] = json::value::array_t();
		result["season_flag"] = 0;

        return result;
	}
}
