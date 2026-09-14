#pragma once

#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_get_svrtime final : public command_handler
	{
	public:
		json::value execute(json::value& data, const std::optional<database::users::user>& player) override;
	};
}
