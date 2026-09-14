#include <std_include.hpp>

#include "cmd_error_lang_id_send.hpp"

// not implemented
namespace emulator::ssd
{
	glz::json cmd_error_lang_id_send::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
