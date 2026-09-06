#include <std_include.hpp>

#include "cmd_communication_gesture_slot_save.hpp"

// not implemented
namespace emulator::ssd
{
	nlohmann::json cmd_communication_gesture_slot_save::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
