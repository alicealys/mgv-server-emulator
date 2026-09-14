#include <std_include.hpp>

#include "cmd_purchase_deploy_reduction.hpp"

// not implemented
namespace emulator::ssd
{
	glz::json cmd_purchase_deploy_reduction::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
