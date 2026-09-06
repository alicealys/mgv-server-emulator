#include <std_include.hpp>

#include "cmd_error_lang_id_send.hpp"

// not implemented
namespace emulator::ssd
{
	nlohmann::json cmd_error_lang_id_send::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
