#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_craft_open_perk final : public command_handler
	{
		struct param_t
		{
			std::uint32_t perk_id;
			std::uint32_t recipe_id;
			std::uint32_t target_item;
		};

		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
