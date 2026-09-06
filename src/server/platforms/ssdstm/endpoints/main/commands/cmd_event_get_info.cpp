#include <std_include.hpp>

#include "cmd_event_get_info.hpp"

namespace emulator::ssd
{
	nlohmann::json cmd_event_get_info::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;

		// TODO
		result["event_list"] = nlohmann::json::array();
		result["season_flag"] = 0;

        return result;
	}
}
