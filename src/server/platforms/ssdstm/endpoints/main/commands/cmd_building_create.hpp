#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_building_create final : public command_handler
	{
		struct param_t
		{
			std::uint32_t production_id;
			std::uint32_t production_num;
			std::uint32_t map_location;
		};

		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
