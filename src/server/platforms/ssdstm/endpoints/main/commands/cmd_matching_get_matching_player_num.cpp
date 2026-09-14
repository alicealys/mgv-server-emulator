#include <std_include.hpp>

#include "cmd_matching_get_matching_player_num.hpp"

// not implemented
namespace emulator::ssd
{
	json::value cmd_matching_get_matching_player_num::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
