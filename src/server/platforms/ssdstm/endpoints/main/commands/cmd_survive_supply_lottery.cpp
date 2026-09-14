#include <std_include.hpp>

#include "cmd_survive_supply_lottery.hpp"

// not implemented
namespace emulator::ssd
{
	glz::json cmd_survive_supply_lottery::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
