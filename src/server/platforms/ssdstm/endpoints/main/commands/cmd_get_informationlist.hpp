#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_get_informationlist final : public command_handler
	{
		struct param_t
		{
			std::string lang;
			std::string region;
		};

		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
	};
}
