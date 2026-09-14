#pragma once

#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_get_urllist final : public command_handler
	{
	public:
		cmd_get_urllist();

		glz::json execute(glz::json& data, const std::optional<database::users::user>& player) override;
	};
}
