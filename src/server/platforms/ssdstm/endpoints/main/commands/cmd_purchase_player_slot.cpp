#include <std_include.hpp>

#include "cmd_purchase_player_slot.hpp"

// not implemented
namespace emulator::ssd
{
	json::value cmd_purchase_player_slot::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
