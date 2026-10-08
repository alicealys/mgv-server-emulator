#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_coop_mission_start final : public command_handler
	{
		struct param_t
		{
			std::uint8_t flag;
			std::uint32_t mission_code;
		};

		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
