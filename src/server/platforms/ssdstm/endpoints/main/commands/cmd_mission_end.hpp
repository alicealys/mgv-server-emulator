#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_mission_end final : public command_handler
	{
		nlohmann::json execute(nlohmann::json& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
