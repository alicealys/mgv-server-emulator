#include <std_include.hpp>

#include "cmd_boost_add_confirm.hpp"

// not implemented
namespace emulator::ssd
{
	json::value cmd_boost_add_confirm::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
