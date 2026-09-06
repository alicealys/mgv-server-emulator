#include <std_include.hpp>

#include "cmd_purchase_additional_storage.hpp"

// not implemented
namespace emulator::ssd
{
	nlohmann::json cmd_purchase_additional_storage::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
