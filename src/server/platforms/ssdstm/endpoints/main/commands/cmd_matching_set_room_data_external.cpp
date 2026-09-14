#include <std_include.hpp>

#include "cmd_matching_set_room_data_external.hpp"

// not implemented
namespace emulator::ssd
{
	json::value cmd_matching_set_room_data_external::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
