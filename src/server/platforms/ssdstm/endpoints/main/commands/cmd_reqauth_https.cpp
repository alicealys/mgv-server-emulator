#include <std_include.hpp>

#include "cmd_reqauth_https.hpp"

// not implemented
namespace emulator::ssd
{
	nlohmann::json cmd_reqauth_https::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
