#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_base_resource_provide final : public command_handler
	{
		struct entry_t
		{
			std::uint16_t inventory_index;
			std::uint8_t inventory_type;
			std::uint16_t num;
		};

		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
