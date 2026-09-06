#include <std_include.hpp>

#include "cmd_maintenance_get_schedule.hpp"

namespace emulator::ssd
{
	nlohmann::json cmd_maintenance_get_schedule::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;

		result["server_time"] = std::time(nullptr);
		result["next_maintenance"] = std::numeric_limits<std::int32_t>::max();
		result["message_type"] = 0;
		result["maintenance_type"] = 0;
		result["maintenance_count"] = 0;

        return result;
	}
}
