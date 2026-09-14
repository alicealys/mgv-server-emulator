#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_gdpr_check final : public command_handler
	{
	public:
		cmd_gdpr_check();
		glz::json execute(glz::json& data, const std::optional<database::users::user>& user) override;

	private:
		glz::json list_;

	};
}
