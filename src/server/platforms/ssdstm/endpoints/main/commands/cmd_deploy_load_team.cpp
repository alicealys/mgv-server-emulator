#include <std_include.hpp>

#include "cmd_deploy_load_team.hpp"

// not implemented
namespace emulator::ssd
{
	glz::json cmd_deploy_load_team::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;

		// TODO

		result["price_list"][0] = 0;
		result["price_list"][1] = 0;
		result["price_list"][2] = 0;
		result["price_list"][3] = 0;
		result["price_list"][4] = 0;
		result["team_list"] = glz::json::array_t();

        return result;
	}
}
