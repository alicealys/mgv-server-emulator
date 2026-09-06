#pragma once

#include "database.hpp"

namespace database
{
	struct vars_t
	{
		std::chrono::seconds session_heartbeat = 60s;
		std::chrono::seconds session_timeout = 200s;
		std::uint32_t server_version = 19;
	};
	
	extern vars_t vars;

	void initialize_vars();
}
