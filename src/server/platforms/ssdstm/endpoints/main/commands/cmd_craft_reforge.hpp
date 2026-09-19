#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_craft_reforge final : public command_handler
	{
		struct param_t
		{
			std::uint16_t inventory_index_target;
			std::uint16_t inventory_index_junk;
			std::uint32_t reforge_type;
		};

		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
