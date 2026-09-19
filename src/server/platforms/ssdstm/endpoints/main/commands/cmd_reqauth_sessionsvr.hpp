#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_reqauth_sessionsvr final : public command_handler
	{
		struct param_t
		{
			std::string dont_check_heartbeat;
			std::string session;
			std::string svrtype;
			std::uint64_t user_id;
		};

		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
