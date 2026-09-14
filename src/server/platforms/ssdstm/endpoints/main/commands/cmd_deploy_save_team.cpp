#include <std_include.hpp>

#include "cmd_deploy_save_team.hpp"

// not implemented
namespace emulator::ssd
{
	glz::json cmd_deploy_save_team::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
