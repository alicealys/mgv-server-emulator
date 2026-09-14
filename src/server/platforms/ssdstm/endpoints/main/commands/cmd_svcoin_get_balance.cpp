#include <std_include.hpp>

#include "cmd_svcoin_get_balance.hpp"

// not implemented
namespace emulator::ssd
{
	glz::json cmd_svcoin_get_balance::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
