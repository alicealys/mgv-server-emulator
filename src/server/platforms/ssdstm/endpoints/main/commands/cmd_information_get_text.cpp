#include <std_include.hpp>

#include "cmd_information_get_text.hpp"

namespace emulator::ssd
{
	json::value cmd_information_get_text::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		// TODO
		result["text"] = "";
		result["url"] = "";

        return result;
	}
}
