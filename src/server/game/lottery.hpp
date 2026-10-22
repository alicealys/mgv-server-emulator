#pragma once

#include "game/game.hpp"

namespace game
{
	using lottery_t = game::item_t(*)();

	extern std::unordered_map<std::uint32_t, lottery_t> battle_pack_lotteries;
	extern std::array<std::unordered_map<std::uint32_t, lottery_t>, 6> ui_reward_lotteries;

	void initialize_lottery();
}
