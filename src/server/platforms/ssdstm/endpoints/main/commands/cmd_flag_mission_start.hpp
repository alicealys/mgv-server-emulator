#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_flag_mission_start final : public command_handler
	{
		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
	};
}
