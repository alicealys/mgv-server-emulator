#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_crew_update final : public command_handler
	{
		struct group_transfer_entry_t
		{
			std::uint16_t group_id;
			std::uint32_t unique_id;
		};

		struct update_nickname_entry_t
		{
			std::string name;
			std::uint32_t unique_id;
		};

		struct update_flag_entry_t
		{
			std::uint32_t flag;
			std::uint32_t unique_id;
		};

		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
