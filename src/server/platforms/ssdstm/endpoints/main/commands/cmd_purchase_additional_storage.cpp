#include <std_include.hpp>

#include "cmd_purchase_additional_storage.hpp"

// not implemented
namespace emulator::ssd
{
	json::value cmd_purchase_additional_storage::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
