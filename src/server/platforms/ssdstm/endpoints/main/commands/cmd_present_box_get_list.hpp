#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_present_box_get_list final : public command_handler
	{
		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
