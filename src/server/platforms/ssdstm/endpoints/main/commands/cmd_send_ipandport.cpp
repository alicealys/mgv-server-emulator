#include <std_include.hpp>

#include "cmd_send_ipandport.hpp"

namespace emulator::ssd
{
	json::value cmd_send_ipandport::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		param_t param{};
		if (!json::read(param, data))
		{
			return error(ERR_INVALIDARG);
		}

		const auto parse_ip = [](const std::string& data)
			-> std::string
		{
			in_addr addr{};
			if (inet_pton(AF_INET, data.data(), &addr) <= 0)
			{
				return {};
			}

			std::string buffer(0x100, 0);
			const auto result = inet_ntop(AF_INET, &addr, buffer.data(), buffer.size());
			if (result == nullptr)
			{
				return {};
			}

			buffer.resize(std::strlen(result));

			return buffer;
		};

		const auto& cfg = config::get();

		if (cfg.vars.store_client_ips)
		{
			database::users::set_ip_and_port(user->get_user_id(),
				parse_ip(param.ex_ip), param.ex_port,
				parse_ip(param.in_ip), param.in_port,
				param.nat
			);
		}

		return result;
	}

	std::uint32_t cmd_send_ipandport::flags()
	{
		return CMD_NEEDS_USER;
	}
}
