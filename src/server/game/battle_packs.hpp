#pragma once

#include "game/game.hpp"

namespace game
{
	using battle_pack_lottery_t = game::item_t(*)();
	extern std::unordered_map<std::uint32_t, battle_pack_lottery_t> battle_pack_lotteries;
	void initialize_battle_packs();
}
