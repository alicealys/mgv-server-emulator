#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_event_get_friend_ranking final : public command_handler
	{
		struct param_t
		{
			struct account_entry_t
			{
				std::uint64_t id;
				std::uint8_t type;
			};

			std::vector<account_entry_t> account_list;
			std::uint64_t event_id;
		};

		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
