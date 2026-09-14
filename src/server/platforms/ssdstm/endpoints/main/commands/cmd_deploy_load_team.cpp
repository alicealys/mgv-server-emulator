#include <std_include.hpp>

#include "cmd_deploy_load_team.hpp"

// not implemented
namespace emulator::ssd
{
	json::value cmd_deploy_load_team::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		// TODO

		result["price_list"][0] = 0;
		result["price_list"][1] = 0;
		result["price_list"][2] = 0;
		result["price_list"][3] = 0;
		result["price_list"][4] = 0;
		result["team_list"] = json::array();

        return result;
	}
}
