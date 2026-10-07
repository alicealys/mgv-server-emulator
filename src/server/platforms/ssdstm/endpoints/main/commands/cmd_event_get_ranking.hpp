#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_event_get_ranking final : public command_handler
	{
		struct param_t
		{
			std::uint64_t event_id;
			std::uint32_t start_rank;
			std::uint32_t num;
		};

		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
