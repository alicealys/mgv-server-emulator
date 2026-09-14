#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_avatar_save final : public command_handler
	{
		glz::json execute(glz::json& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
