#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_defense_mission_start final : public command_handler
	{
		struct param_t
		{
			std::uint16_t mission_code;
			std::uint8_t wave;
		};

		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
