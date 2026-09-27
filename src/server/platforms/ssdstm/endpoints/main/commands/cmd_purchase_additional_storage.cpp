#include <std_include.hpp>

#include "cmd_purchase_additional_storage.hpp"

namespace emulator::ssd
{
	json::value cmd_purchase_additional_storage::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;
		
		std::uint32_t additional_storage_type{};
		if (!json::read(additional_storage_type, data["additional_storage_type"]))
		{
			return error(ERR_INVALIDARG);
		}

		result["purchase_result"]["is_coin"] = 0;
		result["purchase_result"]["payment"] = 0;
		result["purchase_result"]["balance"] = 0;
		result["additional_storage_type"] = 0;
		result["result"] = "ERR_NOTIMPLEMENTED";

		return result;
	}

	std::uint32_t cmd_purchase_additional_storage::flags()
	{
		return CMD_NEEDS_USER;
	}
}
