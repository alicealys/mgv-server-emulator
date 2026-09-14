#include <std_include.hpp>

#include "cmd_communication_gesture_slot_save.hpp"

// not implemented
namespace emulator::ssd
{
	glz::json cmd_communication_gesture_slot_save::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
