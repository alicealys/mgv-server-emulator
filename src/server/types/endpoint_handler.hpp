#pragma once

#include "base_handler.hpp"
#include "command_handler.hpp"
#include "utils/http_server.hpp"

namespace emulator
{
	class endpoint_handler : public base_handler<command_handler>
	{
	public:
		virtual ~endpoint_handler()
		{
		}

		virtual std::optional<json::value> decrypt_request(const std::string& data, std::optional<database::users::user>& player)
		{
			return {};
		}

		virtual bool verify_request(json::value& request)
		{
			return false;
		}

		virtual bool encrypt_response(json::value& request, json::value& data, const std::optional<database::users::user>& player, std::string& result)
		{
			return false;
		}

		virtual void handle_command(const utils::request_params& request, utils::response_params& response);

		void print_handler_name([[ maybe_unused ]] const std::string& name) override;

		void set_platform(const std::string& platform);

	private:
		std::string platform_;

	};
}
