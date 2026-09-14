#include <std_include.hpp>

#include "cmd_server_data_initialize.hpp"

// not implemented
namespace emulator::ssd
{
	json::value cmd_server_data_initialize::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
