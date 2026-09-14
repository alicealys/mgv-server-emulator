#include <std_include.hpp>

#include "cmd_information_get_text.hpp"

namespace emulator::ssd
{
	glz::json cmd_information_get_text::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;

		// TODO
		result["text"] = "";
		result["url"] = "";

        return result;
	}
}
