#include <std_include.hpp>

#include "status.hpp"
#include "database/auth.hpp"
#include "database/models/players.hpp"
#include "server.hpp"

#include <utils/http.hpp>
#include <version.h>

namespace emulator
{
	nlohmann::json status::handle_request(const utils::request_params& params)
	{
		nlohmann::json result;

		const auto now = std::chrono::system_clock::now();
		const auto uptime = now - get_server_startup();
		const auto uptime_s = std::chrono::duration_cast<std::chrono::seconds>(uptime).count();

		result["uptime"] = uptime_s;
		result["version"] = VERSION;

		return result;
	}
}
