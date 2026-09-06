#include <std_include.hpp>

#include "cmd_present_box_get_item.hpp"

// not implemented
namespace emulator::ssd
{
	nlohmann::json cmd_present_box_get_item::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
