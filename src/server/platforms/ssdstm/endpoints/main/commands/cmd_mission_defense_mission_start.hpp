#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_mission_defense_mission_start final : public command_handler
	{
		struct param_t
		{
			std::uint32_t attack_time;
			std::uint32_t flag;
			std::uint32_t threat;
			std::uint32_t threat_threshold;
		};

		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
