#include <std_include.hpp>

#include "cmd_dlc_get_flag.hpp"

namespace emulator::ssd
{
	nlohmann::json cmd_dlc_get_flag::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;

		// TODO
		result["consumable_count"] = {0, 0, 0, 0, 0, 0, 0, 0};
        result["flag"] = 3759946752;

        return result;
	}
}
