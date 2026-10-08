#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_matching_kick_member final : public command_handler
	{
		struct param_t
		{
			std::uint64_t room_id;
			std::uint64_t first_party_id;
		};

		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
