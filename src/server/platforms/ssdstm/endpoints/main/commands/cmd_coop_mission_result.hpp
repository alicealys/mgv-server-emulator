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

		struct reward_settings_t
		{
			using point_table_entry_t = std::array<std::uint32_t, 6>;

			std::unordered_map<std::uint32_t, point_table_entry_t> event_point_table;
		};

		cmd_coop_mission_result();
		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;

	private:
		reward_settings_t reward_settings_{};

	};
}
