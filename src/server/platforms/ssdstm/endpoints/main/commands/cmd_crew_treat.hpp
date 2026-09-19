#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_crew_treat final : public command_handler
	{
		struct entry_t
		{
			std::uint32_t unique_id;
			std::uint32_t treat_item_injury_1;
			std::uint32_t treat_item_injury_2;
			std::uint32_t treat_item_sickness_1;
			std::uint32_t treat_item_sickness_2;
		};

		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
