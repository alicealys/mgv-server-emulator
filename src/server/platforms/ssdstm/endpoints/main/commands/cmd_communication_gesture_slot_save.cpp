#include <std_include.hpp>

#include "cmd_communication_gesture_slot_save.hpp"

namespace emulator::ssd
{
	json::value cmd_communication_gesture_slot_save::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		database::players::communication_gesture_info_t info{};
		if (!info.parse(data["communication_gesture_slot"]))
		{
			return error(ERR_INVALIDARG);
		}

		user->current_player->set_communication_gesture_info(info);

        return result;
	}

	std::uint32_t cmd_communication_gesture_slot_save::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
