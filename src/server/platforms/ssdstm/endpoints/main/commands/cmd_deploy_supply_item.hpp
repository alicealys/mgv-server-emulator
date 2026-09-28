#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_deploy_supply_item final : public command_handler
	{
		struct entry_t
		{
			struct storage_t
			{
				std::uint16_t count;
				std::uint16_t index;
				std::uint8_t type;
			};

			std::uint8_t index;
			std::uint8_t is_delete;
			std::uint8_t is_new;
			storage_t storage;
		};

		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
