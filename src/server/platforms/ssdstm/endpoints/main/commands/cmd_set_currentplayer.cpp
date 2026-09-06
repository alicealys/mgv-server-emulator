#include <std_include.hpp>

#include "cmd_set_currentplayer.hpp"

// not implemented
namespace emulator::ssd
{
	nlohmann::json cmd_set_currentplayer::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
