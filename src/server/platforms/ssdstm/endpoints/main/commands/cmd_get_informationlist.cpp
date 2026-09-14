#include <std_include.hpp>

#include "cmd_get_informationlist.hpp"

// not implemented
namespace emulator::ssd
{
	json::value cmd_get_informationlist::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
