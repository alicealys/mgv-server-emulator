#include <std_include.hpp>

#include "cmd_get_svrlist.hpp"

namespace emulator::ssd
{
	json::value cmd_get_svrlist::execute(json::value& data, const std::optional<database::users::user>&)
	{
		json::value result;

		result["server_num"] = 0;
		result["svrlist"] = json::array{};

		return result;
	}
}