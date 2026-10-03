#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_base_resource_draw_water final : public command_handler
	{
		struct param_t
		{
			std::uint32_t clean_water;
			std::uint32_t dirty_water;
		};

		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
