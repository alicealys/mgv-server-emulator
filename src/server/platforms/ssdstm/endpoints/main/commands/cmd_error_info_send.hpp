#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_error_info_send final : public command_handler
	{
		struct param_t
		{
			std::uint32_t action;
			std::uint32_t error;
			std::uint32_t info;
			std::string message;
		};

		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
