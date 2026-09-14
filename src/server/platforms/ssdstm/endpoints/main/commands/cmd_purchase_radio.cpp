#include <std_include.hpp>

#include "cmd_purchase_radio.hpp"

// not implemented
namespace emulator::ssd
{
	glz::json cmd_purchase_radio::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
