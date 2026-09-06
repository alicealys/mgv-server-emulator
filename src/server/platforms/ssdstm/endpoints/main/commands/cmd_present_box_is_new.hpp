#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_present_box_is_new final : public command_handler
	{
		nlohmann::json execute(nlohmann::json& data, const std::optional<database::users::user>& user) override;
	};
}
