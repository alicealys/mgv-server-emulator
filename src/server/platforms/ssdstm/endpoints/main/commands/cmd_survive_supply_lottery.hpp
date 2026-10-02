#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_survive_supply_lottery final : public command_handler
	{
		struct param_t
		{
			std::uint32_t base_exp_level;
			std::uint8_t is_wormhole_portal_obtained;
		};

		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
