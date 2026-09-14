#include <std_include.hpp>

#include "cmd_present_box_get_all.hpp"

// not implemented
namespace emulator::ssd
{
	json::value cmd_present_box_get_all::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
