#include <std_include.hpp>

#include "cmd_matching_set_room_data_external.hpp"

// not implemented
namespace emulator::ssd
{
	nlohmann::json cmd_matching_set_room_data_external::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
