#include <std_include.hpp>

#include "cmd_server_data_initialize.hpp"

// not implemented
namespace emulator::ssd
{
	glz::json cmd_server_data_initialize::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
