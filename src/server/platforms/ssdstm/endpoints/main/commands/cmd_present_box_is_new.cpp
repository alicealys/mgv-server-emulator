#include <std_include.hpp>

#include "cmd_present_box_is_new.hpp"

namespace emulator::ssd
{
	json::value cmd_present_box_is_new::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		// TODO
        result["is_new"] = 0;

        return result;
	}
}
