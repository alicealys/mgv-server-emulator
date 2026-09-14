#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_gdpr_check final : public command_handler
	{
	public:
		cmd_gdpr_check();
		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;

	private:
		json::value list_;

	};
}
