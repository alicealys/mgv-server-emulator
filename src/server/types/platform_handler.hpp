#pragma once

#include "base_handler.hpp"
#include "endpoint_handler.hpp"

namespace emulator
{
	class platform_handler : public base_handler<endpoint_handler>
	{
	public:
		void handle_endpoint(const std::string& endpoint, const utils::request_params& request, utils::response_params& response);
		void print_handler_name([[ maybe_unused ]] const std::string& name) override;
	};
}
