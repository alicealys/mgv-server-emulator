#include <std_include.hpp>

#include "platform_handler.hpp"

#include "component/console.hpp"

namespace emulator
{
	void platform_handler::handle_endpoint(const std::string& endpoint, const utils::request_params& request, utils::response_params& response)
	{
		const auto handler = this->handlers_.find(endpoint);
		if (handler == this->handlers_.end())
		{
			return;
		}

		try
		{
			handler->second->handle_command(request, response);
		}
		catch (const std::exception& e)
		{
			console::error("Error handling command: %s\n", e.what());
		}
	}

	void platform_handler::print_handler_name([[ maybe_unused ]] const std::string& name)
	{
		console::log("Registering endpoint \"%s\"\n", name.data());
	}
}
