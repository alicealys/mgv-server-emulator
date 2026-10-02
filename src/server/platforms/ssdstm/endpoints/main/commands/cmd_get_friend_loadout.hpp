#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_get_friend_loadout final : public command_handler
	{
		struct param_t
		{
			struct target_t
			{
				std::uint64_t id;
				std::uint8_t type;
			};

			target_t target;
		};

		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
