#include <std_include.hpp>

#include "vars.hpp"

namespace database
{
	vars_t vars{};

	void initialize_vars()
	{
		vars.session_heartbeat = 1s * config::get_or<std::uint64_t>("vars.session_heartbeat", vars.session_heartbeat.count());
		vars.session_timeout = 1s * config::get_or<std::uint64_t>("vars.session_timeout", vars.session_timeout.count());

		vars.server_version = config::get_or<std::int32_t>("vars.server_version", vars.server_version);
	}
}
