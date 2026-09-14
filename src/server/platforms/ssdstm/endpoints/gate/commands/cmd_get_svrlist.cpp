#include <std_include.hpp>

#include "cmd_get_svrlist.hpp"

namespace emulator::ssd
{
	glz::json cmd_get_svrlist::execute(glz::json& data, const std::optional<database::users::user>&)
	{
		glz::json result;

		result["result"] = "NOERR";
		result["server_num"] = 0;
		result["svrlist"] = glz::json::array_t{};
		result["xuid"] = glz::json::object_t{};

		return result;
	}
}