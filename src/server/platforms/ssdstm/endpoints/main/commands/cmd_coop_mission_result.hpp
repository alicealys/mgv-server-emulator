#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_coop_mission_result final : public command_handler
	{
	public:
		struct param_t
		{
			std::uint8_t clear_type;
			std::uint8_t event_flag;
			std::uint32_t mission_id;
			std::uint8_t rank;
			std::uint8_t rescue;
			std::uint8_t rule_type;
			std::uint16_t story_sequence_number;
			std::uint32_t personal_score;
			std::uint32_t score;
			std::uint8_t waves;
		};

		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;

	};
}
