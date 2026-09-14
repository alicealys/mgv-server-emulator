#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_steam_shop_open final : public command_handler
	{
		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
	};
}
