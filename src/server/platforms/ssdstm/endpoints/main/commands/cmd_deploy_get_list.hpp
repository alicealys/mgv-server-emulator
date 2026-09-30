#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_deploy_get_list final : public command_handler
	{
		struct param_t
		{
			std::uint32_t level;
			std::uint32_t location;
		};

		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
