#include <std_include.hpp>

#include "cmd_event_get_info.hpp"

namespace emulator::ssd
{
	glz::json cmd_event_get_info::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;

		// TODO
		result["event_list"] = glz::json::array_t();
		result["season_flag"] = 0;

        return result;
	}
}
