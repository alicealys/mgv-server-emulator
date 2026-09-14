#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_server_data_initialize final : public command_handler
	{
		glz::json execute(glz::json& data, const std::optional<database::users::user>& user) override;
	};
}
