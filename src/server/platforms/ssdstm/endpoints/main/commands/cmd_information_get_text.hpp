#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_information_get_text final : public command_handler
	{
		struct param_t
		{
			std::uint8_t lang;
			std::uint8_t is_note;
			std::uint32_t info_id;
		};

		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
	};
}
