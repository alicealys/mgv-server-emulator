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
				struct int_entry_t
				{
					std::uint8_t index;
					std::uint32_t value;
				};

				struct bin_entry_t
				{
					std::uint8_t index;
					std::string data;
				};

				std::vector<bin_entry_t> room_searchable_bin_attr_external;
				std::vector<int_entry_t> room_searchable_int_attr_external;
				std::uint8_t region_matching_level;
				std::uint64_t room_id;
			};

			room_data_t room_data;
		};

		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
