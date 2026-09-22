#include <std_include.hpp>

#include "vars.hpp"

namespace database
{
	vars_t vars{};

	void initialize_vars()
	{
		auto& cfg = config::get();

		vars.session_heartbeat = 1s * cfg.vars.session_heartbeat;
		vars.session_timeout = 1s * cfg.vars.session_timeout;
		vars.server_version = cfg.vars.server_version;
		vars.disable_tutorial_tips = cfg.vars.disable_tutorial_tips;
	}
}
