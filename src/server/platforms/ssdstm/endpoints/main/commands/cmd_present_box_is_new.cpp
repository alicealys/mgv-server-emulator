#include <std_include.hpp>

#include "cmd_present_box_is_new.hpp"

namespace emulator::ssd
{
	glz::json cmd_present_box_is_new::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;

		// TODO
        result["is_new"] = 0;

        return result;
	}
}
