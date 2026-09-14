#include <std_include.hpp>

#include "cmd_coop_lobby_end.hpp"

// not implemented
namespace emulator::ssd
{
	json::value cmd_coop_lobby_end::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
