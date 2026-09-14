#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_server_parameter_load final : public command_handler
	{
	public:
		cmd_server_parameter_load();
		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;

	private:
		json::value list_;

	};
}
