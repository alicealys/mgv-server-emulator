#include <std_include.hpp>

#include "cmd_information_get_title.hpp"

namespace emulator::ssd
{
	nlohmann::json cmd_information_get_title::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;
		
		// TODO
		result["info_list"] = nlohmann::json::array();
		result["info_num"] = 0;

        return result;
	}
}
