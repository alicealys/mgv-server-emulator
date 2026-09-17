#include <std_include.hpp>

#include "cmd_craft_deliver.hpp"

struct param_t
{
    std::uint32_t bad_status_1_risk;
    std::uint32_t bad_status_2_risk;
    std::uint32_t bad_status_3_risk;
    std::uint32_t bad_status_4_risk;
    std::uint32_t clean_water;
    std::uint32_t food;
    std::uint32_t inventory_index;
    std::uint32_t inventory_index2;
    std::uint32_t inventory_type;
    std::uint32_t inventory_type2;
    std::uint32_t list_type;
    std::uint32_t medical_supplies;
    std::uint32_t num;
};

namespace emulator::ssd
{
	json::value cmd_craft_deliver::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

        param_t param{};
        if (json::read(param, data))
        {
            return error(ERR_INVALIDARG);
        }

        result["gain_resources"] = json::array();
        result["gain_stackables"] = json::array();
        result["result"] = "ERR_NOTIMPLEMENTED";

        return result;
	}

	std::uint32_t cmd_craft_deliver::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
