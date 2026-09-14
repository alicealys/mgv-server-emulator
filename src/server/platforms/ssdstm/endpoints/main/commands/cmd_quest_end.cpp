#include <std_include.hpp>

#include "cmd_quest_end.hpp"

// not implemented
namespace emulator::ssd
{
	json::value cmd_quest_end::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
