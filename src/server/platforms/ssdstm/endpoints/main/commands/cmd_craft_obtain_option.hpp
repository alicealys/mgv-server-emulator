#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_craft_obtain_option final : public command_handler
	{
		struct param_t
		{
			std::uint16_t inventory_index;
			std::uint8_t inventory_type;
			std::uint32_t option_id;
			std::uint8_t option_slot;
		};

		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
