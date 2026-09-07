#include <std_include.hpp>

#include "endpoint_handler.hpp"

#include "component/console.hpp"

namespace emulator
{
	std::optional<std::string> endpoint_handler::handle_command(const utils::request_params& params)
	{
		std::optional<database::users::user> user;
		auto json_req_opt = this->decrypt_request(params.body, user);
		if (!json_req_opt.has_value())
		{
			return {};
		}

		auto& json_req = json_req_opt.value();
		if (!this->verify_request(json_req))
		{
			return {};
		}

		const auto& session_key = json_req["session_key"];
		if (!session_key.is_string())
		{
			return {};
		}

		const auto msgid_str = json_req["data"]["msgid"].get<std::string>();
		const auto handler = this->handlers_.find(msgid_str);

		if (handler == this->handlers_.end())
		{
			console::warning("[Endpoint %s] Missing handler for \"%s\"\n", this->platform_.data(), msgid_str.data());
			return {};
		}

#ifdef DEBUG
		static std::atomic_int64_t exec_id = 0;
		auto id = exec_id++;
		const auto start = std::chrono::high_resolution_clock::now();
		const auto _0 = gsl::finally([=]
		{
			const auto end = std::chrono::high_resolution_clock::now();
			const auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
			console::debug("[Endpoint] Took %lli msec (%lli)", diff, id);
		});

		console::debug("[Endpoint] Handling command \"%s\" (%lli)\n", msgid_str.data(), id);
#endif

		auto get_json_response = [&]
		{
			const auto flags = handler->second->flags();
			if ((flags & CMD_NEEDS_USER) && !user.has_value())
			{
				return error(ERR_INVALID_SESSION);
			}

			if ((flags & CMD_NEEDS_PLAYER) && (!user.has_value() || !user->current_player.has_value()))
			{
				return error(ERR_INVALID_SESSION);
			}

			return handler->second->execute(json_req["data"], user);
		};

		auto json_res = get_json_response();
		auto& result_j = json_res["result"];

		if (!result_j.is_string())
		{
			result_j = "NOERR";
		}

#ifdef DEBUG
		const auto result = result_j.get<std::string>();
		console::debug("[Endpoint] Command \"%s\" (%lli) result: %s\n", msgid_str.data(), id, result.data());
#endif

		return this->encrypt_response(json_req, json_res, user);
	}

	void endpoint_handler::print_handler_name([[ maybe_unused ]] const std::string& name)
	{
		console::log("Registering command for \"%s\": \"%s\"\n", this->platform_.data(), name.data());
	}

	void endpoint_handler::set_platform(const std::string& platform)
	{
		this->platform_ = platform;
	}
}
