#include <std_include.hpp>

#include "var_handler.hpp"

namespace emulator
{
	var_handler::var_handler(const std::string& name, const std::string& value)
		: name_(name)
		  , value_(value)
	{
	}

	void var_handler::handle_command(const utils::request_params& request, utils::response_params& response)
	{
		response.headers.append("Content-Type: text/plain\n");

		if (request.uri == "/ssdstmweb/"s + this->name_)
		{
			response.body = this->value_;
		}
	}
}
