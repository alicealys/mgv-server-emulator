#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_send_ipandport final : public command_handler
	{
		struct param_t
		{
			std::string nat;
			std::string ex_ip;
			std::string in_ip;
			std::uint16_t ex_port;
			std::uint16_t in_port;
		};

		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
