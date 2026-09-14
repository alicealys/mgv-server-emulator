#include <std_include.hpp>

#include "cmd_information_get_detail.hpp"

namespace emulator::ssd
{
	json::value cmd_information_get_detail::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		// TODO
		result["title"]["text"] = "";
		result["title"]["release_date"] = 0;
		result["title"]["update_date"] = 0;
		result["title"]["important"] = 0;
		result["title"]["personal"] = 0;
		result["title"]["info_id"] = 0;
		result["url"] = "";

        return result;
	}
}
