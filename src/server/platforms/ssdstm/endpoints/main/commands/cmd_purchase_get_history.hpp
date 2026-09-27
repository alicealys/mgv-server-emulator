#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_purchase_get_history final : public command_handler
	{
		struct param_t
		{
			std::uint32_t num;
			std::uint32_t start;
		};

		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
