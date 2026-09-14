#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_event_get_around_ranking final : public command_handler
	{
		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
	};
}
