#include <std_include.hpp>

#include "endpoints/steam_openid.hpp"
#include "endpoints/steam_openid_url.hpp"
#include "endpoints/status.hpp"
#include "endpoints/delete_account.hpp"
#include "utils/config.hpp"

#include "api_handler.hpp"

namespace emulator
{
	void api_endpoint::handle_command(const utils::request_params& request, utils::response_params& response)
	{
		const auto result = this->handle_request(request);
		response.body = result.dump();
		response.headers.append("Content-Type: application/json\n");
	}

	api_handler::api_handler()
	{
		const auto enable = config::get<bool>("enable_web_api");
		if (!enable)
		{
			return;
		}

		this->register_handler<steam_openid>("steam_openid");
		this->register_handler<steam_openid_url>("steam_openid_url");
		this->register_handler<status>("status");
		this->register_handler<delete_account>("delete_account");
	}
}
