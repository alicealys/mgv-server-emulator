#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_matching_set_room_data_external final : public command_handler
	{
		struct param_t
		{
			struct room_data_t
			{
				std::uint8_t region_matching_level;
				std::uint64_t room_id;
			};

			room_data_t room_data;
		};

		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
