#include <std_include.hpp>

#include "cmd_maintenance_get_schedule.hpp"

namespace emulator::ssd
{
	glz::json cmd_maintenance_get_schedule::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;

		result["server_time"] = std::time(nullptr);
		result["next_maintenance"] = std::numeric_limits<std::int32_t>::max();
		result["message_type"] = 0;
		result["maintenance_type"] = 0;
		result["maintenance_count"] = 0;

        return result;
	}
}
