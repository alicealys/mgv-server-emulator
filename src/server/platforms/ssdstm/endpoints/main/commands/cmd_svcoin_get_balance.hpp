#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_svcoin_get_balance final : public command_handler
	{
		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
	};
}
