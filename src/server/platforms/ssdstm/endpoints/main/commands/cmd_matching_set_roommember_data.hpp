#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_matching_set_roommember_data final : public command_handler
	{
		struct param_t
		{
			struct data_t
			{
				std::string data;
			};

			struct roommember_data_t
			{
				data_t room_member_bin_attr_internal;
			};

			roommember_data_t roommember_data;
		};

		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
