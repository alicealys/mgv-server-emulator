#include <std_include.hpp>

#include "cmd_information_get_title.hpp"

namespace emulator::ssd
{
	json::value cmd_information_get_title::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;
		
		// TODO
		result["info_list"] = json::value::array_t();
		result["info_num"] = 0;

        return result;
	}
}
