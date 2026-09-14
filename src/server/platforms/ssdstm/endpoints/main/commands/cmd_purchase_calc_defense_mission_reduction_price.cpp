#include <std_include.hpp>

#include "cmd_purchase_calc_defense_mission_reduction_price.hpp"

// not implemented
namespace emulator::ssd
{
	glz::json cmd_purchase_calc_defense_mission_reduction_price::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
