#include <std_include.hpp>

#include "cmd_information_get_detail.hpp"

namespace emulator::ssd
{
	nlohmann::json cmd_information_get_detail::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;

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
