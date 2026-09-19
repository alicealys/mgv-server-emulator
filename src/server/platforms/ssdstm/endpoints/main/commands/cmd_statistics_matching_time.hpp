#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_statistics_matching_time final : public command_handler
	{
		struct param_t
		{
			std::uint32_t cancel_flag;
			std::uint32_t flag;
			std::uint32_t matching_time;
			std::uint32_t member_num;
			std::uint32_t region_level;
			std::uint32_t room_id;
			std::uint32_t rtt_reject;
			std::uint32_t search_num;
		};

		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
