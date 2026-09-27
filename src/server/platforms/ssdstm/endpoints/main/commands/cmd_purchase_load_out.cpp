#include <std_include.hpp>

#include "cmd_purchase_load_out.hpp"

namespace emulator::ssd
{
	json::value cmd_purchase_load_out::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		result["purchase_result"]["is_coin"] = 0;
		result["purchase_result"]["payment"] = 0;
		result["purchase_result"]["balance"] = 0;
		result["index"] = 0;
		result["result"] = "ERR_NOTIMPLEMENTED";

		return result;
	}

	std::uint32_t cmd_purchase_load_out::flags()
	{
		return CMD_NEEDS_USER;
	}
}
