#include <std_include.hpp>

#include "cmd_matching_get_roommember_data.hpp"

// not implemented
namespace emulator::ssd
{
	nlohmann::json cmd_matching_get_roommember_data::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
