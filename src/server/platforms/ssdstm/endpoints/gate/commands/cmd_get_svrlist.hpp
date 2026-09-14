#pragma once

#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_get_svrlist final : public command_handler
	{
		glz::json execute(glz::json& data, const std::optional<database::users::user>& player) override;
	};
}
