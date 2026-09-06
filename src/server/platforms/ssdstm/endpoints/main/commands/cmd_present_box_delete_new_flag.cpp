#include <std_include.hpp>

#include "cmd_present_box_delete_new_flag.hpp"

// not implemented
namespace emulator::ssd
{
	nlohmann::json cmd_present_box_delete_new_flag::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
