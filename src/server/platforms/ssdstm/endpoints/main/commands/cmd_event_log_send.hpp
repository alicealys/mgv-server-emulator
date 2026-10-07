#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_event_log_send final : public command_handler
	{
		struct param_t
		{
			std::string data;
			std::uint32_t mission_id;
			std::uint32_t result_type;
			std::uint32_t size;
			std::uint32_t version;
		};

		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
