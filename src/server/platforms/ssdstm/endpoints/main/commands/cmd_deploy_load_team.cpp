#include <std_include.hpp>

#include "cmd_deploy_load_team.hpp"

// not implemented
namespace emulator::ssd
{
	nlohmann::json cmd_deploy_load_team::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;

		// TODO

		result["price_list"][0] = 0;
		result["price_list"][1] = 0;
		result["price_list"][2] = 0;
		result["price_list"][3] = 0;
		result["price_list"][4] = 0;
		result["team_list"] = nlohmann::json::array();

        return result;
	}
}
