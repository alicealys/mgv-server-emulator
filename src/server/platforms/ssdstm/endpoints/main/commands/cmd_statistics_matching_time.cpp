#include <std_include.hpp>

#include "cmd_statistics_matching_time.hpp"

// not implemented
namespace emulator::ssd
{
	glz::json cmd_statistics_matching_time::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
