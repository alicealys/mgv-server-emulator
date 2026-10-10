#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_boost_add_confirm final : public command_handler
	{
		struct param_t
		{
			std::uint8_t type;
			std::uint32_t hour;
		};

		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
