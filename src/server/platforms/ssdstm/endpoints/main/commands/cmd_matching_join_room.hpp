#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_matching_join_room final : public command_handler
	{
		struct param_t
		{
			std::uint32_t room_id;
			std::uint32_t into_reserve_slot;
			std::string password;
		};

		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
