#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_server_parameter_get_url_list final : public command_handler
	{
	public:
		glz::json execute(glz::json& data, const std::optional<database::users::user>& user) override;

	private:
		glz::json list_;

	};
}
