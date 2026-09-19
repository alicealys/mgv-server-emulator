#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_mission_replay_end final : public command_handler
	{
		struct param_t
		{
			std::uint16_t difficulty;
			std::uint8_t is_clear;
			std::uint16_t mission_code;
		};

		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
