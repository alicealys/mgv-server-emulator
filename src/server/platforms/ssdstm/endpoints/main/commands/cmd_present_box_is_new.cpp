#include <std_include.hpp>

#include "cmd_present_box_is_new.hpp"

namespace emulator::ssd
{
	nlohmann::json cmd_present_box_is_new::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;

		// TODO
        result["is_new"] = 0;

        return result;
	}
}
