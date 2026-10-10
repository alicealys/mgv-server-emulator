#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_boost_check_active final : public command_handler
	{
		struct param_t
		{
			std::uint8_t flag;
			std::uint32_t rate;
		};

		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
