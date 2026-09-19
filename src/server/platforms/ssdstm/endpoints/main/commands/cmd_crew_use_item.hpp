#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_crew_use_item final : public command_handler
	{
		struct entry_t
		{
			std::uint32_t item_resource_id;
			std::uint32_t num;
		};

		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
