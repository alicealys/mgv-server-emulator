#pragma once
#include "types/command_handler.hpp"

#include "game/game.hpp"
#include "database/models/defense_missions.hpp"

namespace emulator::ssd
{
	class cmd_checkpoint_save final : public command_handler
	{
		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;

	private:
		void save_defense_mission(json::value& result, json::value& data, const std::optional<database::users::user>& user);
		void generate_defense_mission_rewards(json::value& result, const std::optional<database::users::user>& user,
			database::defense_missions::reward_list_t& reward_list,
			const game::defense_mission_settings_t& mission_settings, const std::uint8_t rank);

	};
}
