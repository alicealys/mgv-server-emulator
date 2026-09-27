#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_steam_shop_open final : public command_handler
	{
		struct param_t
		{
			std::string lang;
			std::uint32_t steam_item_id;
		};

		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
