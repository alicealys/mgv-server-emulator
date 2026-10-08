#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_matching_set_room_owner final : public command_handler
	{
		struct param_t
		{
			std::uint64_t new_owner_player_id;
			std::uint64_t room_id;
		};

		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
