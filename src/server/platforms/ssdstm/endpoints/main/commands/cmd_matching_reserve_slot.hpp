#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_matching_reserve_slot final : public command_handler
	{
		struct param_t
		{
			std::uint64_t room_id;
			std::uint8_t reserve_num;
		};

		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
