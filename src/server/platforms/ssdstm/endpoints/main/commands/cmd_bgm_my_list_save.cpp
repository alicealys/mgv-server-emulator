#include <std_include.hpp>

#include "cmd_bgm_my_list_save.hpp"

// not implemented
namespace emulator::ssd
{
	json::value cmd_bgm_my_list_save::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
