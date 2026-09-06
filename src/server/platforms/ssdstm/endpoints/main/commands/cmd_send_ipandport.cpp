#include <std_include.hpp>

#include "cmd_send_ipandport.hpp"

namespace emulator::ssd
{
	nlohmann::json cmd_send_ipandport::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;

		const auto& ex_ip_j = data["ex_ip"];
		const auto& in_ip_j = data["in_ip"];
		const auto& ex_port_j = data["ex_port"];
		const auto& in_port_j = data["in_port"];
		const auto& nat_j = data["nat"];

		if (!ex_ip_j.is_string() || !in_ip_j.is_string() || !nat_j.is_string() ||
			!ex_port_j.is_number_unsigned() || !in_port_j.is_number_unsigned())
		{
			return error(ERR_INVALIDARG);
		}

		const auto ex_ip = ex_ip_j.get<std::string>();
		const auto ex_port = ex_port_j.get<std::uint16_t>();
		const auto in_ip = in_ip_j.get<std::string>();
		const auto in_port = in_port_j.get<std::uint16_t>();
		const auto nat = nat_j.get<std::string>();

		database::users::set_ip_and_port(user->get_user_id(),
			ex_ip, ex_port, in_ip, in_port, nat
		);

        return result;
	}
}
