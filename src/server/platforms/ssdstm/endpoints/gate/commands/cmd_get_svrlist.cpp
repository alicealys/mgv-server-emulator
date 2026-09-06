#include <std_include.hpp>

#include "cmd_get_svrlist.hpp"

namespace emulator::ssd
{
	nlohmann::json cmd_get_svrlist::execute(nlohmann::json& data, const std::optional<database::users::user>&)
	{
		nlohmann::json result;

		result["result"] = "NOERR";
		result["server_num"] = 0;
		result["svrlist"] = nlohmann::json::array();
		result["xuid"] = {};

		return result;
	}
}