#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_event_catalog_reward_purchase final : public command_handler
	{
		glz::json execute(glz::json& data, const std::optional<database::users::user>& user) override;
	};
}
