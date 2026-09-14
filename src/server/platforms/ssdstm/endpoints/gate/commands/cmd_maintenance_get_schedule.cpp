#include <std_include.hpp>

#include "cmd_maintenance_get_schedule.hpp"

namespace emulator::ssd
{
	json::value cmd_maintenance_get_schedule::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		result["server_time"] = std::time(nullptr);
		result["next_maintenance"] = std::numeric_limits<std::int32_t>::max();
		result["message_type"] = 0;
		result["maintenance_type"] = 0;
		result["maintenance_count"] = 0;

        return result;
	}
}
