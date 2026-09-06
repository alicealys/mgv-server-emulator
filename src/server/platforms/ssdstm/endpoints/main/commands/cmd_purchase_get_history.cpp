#include <std_include.hpp>

#include "cmd_purchase_get_history.hpp"

// not implemented
namespace emulator::ssd
{
	nlohmann::json cmd_purchase_get_history::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
