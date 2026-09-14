#include <std_include.hpp>

#include "cmd_send_statistics_data.hpp"

// not implemented
namespace emulator::ssd
{
	glz::json cmd_send_statistics_data::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
