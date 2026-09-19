#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_craft_invest_to_player final : public command_handler
	{
		struct param_t
		{
			std::uint32_t cls;
			std::uint32_t energy;
		};

		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
