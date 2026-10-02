#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_wicked_report final : public command_handler
	{
		struct param_t
		{
			struct target_t
			{
				std::uint64_t id;
				std::uint32_t type;
			};

			target_t target;
			std::vector<std::uint32_t> report_list;
		};

		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
