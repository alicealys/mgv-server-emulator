#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_send_statistics_data final : public command_handler
	{
		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
