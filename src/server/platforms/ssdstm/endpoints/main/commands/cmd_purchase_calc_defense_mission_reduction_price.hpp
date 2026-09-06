#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_purchase_calc_defense_mission_reduction_price final : public command_handler
	{
		nlohmann::json execute(nlohmann::json& data, const std::optional<database::users::user>& user) override;
	};
}
