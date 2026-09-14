#include <std_include.hpp>

#include "cmd_information_get_title.hpp"

namespace emulator::ssd
{
	glz::json cmd_information_get_title::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;
		
		// TODO
		result["info_list"] = glz::json::array_t();
		result["info_num"] = 0;

        return result;
	}
}
