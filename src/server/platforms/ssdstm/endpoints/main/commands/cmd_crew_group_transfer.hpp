#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_crew_group_transfer final : public command_handler
	{
		struct entry_t
		{
			std::uint32_t unique_id;
			std::uint16_t group_id;
		};

		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
