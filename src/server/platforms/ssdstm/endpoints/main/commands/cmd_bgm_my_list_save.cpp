#include <std_include.hpp>

#include "cmd_bgm_my_list_save.hpp"

// not implemented
namespace emulator::ssd
{
	glz::json cmd_bgm_my_list_save::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
