#include <std_include.hpp>

#include "cmd_information_get_text.hpp"

namespace emulator::ssd
{
	nlohmann::json cmd_information_get_text::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;

		// TODO
		result["text"] = "";
		result["url"] = "";

        return result;
	}
}
