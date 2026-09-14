#include <std_include.hpp>

#include "cmd_event_reward_receive.hpp"

// not implemented
namespace emulator::ssd
{
	json::value cmd_event_reward_receive::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		// TODO
		result["reward"]["energy"] = 0;
		result["reward"]["kub_boost_flag"] = 0;
		result["reward"]["present_box_num"] = 0;
		result["reward"]["battle_pack_list"] = json::value::array_t();
		result["reward"]["nonstackable_list"] = json::value::array_t();
		result["reward"]["present_list"] = json::value::array_t();
		result["reward"]["recipe_list"] = json::value::array_t();
		result["reward"]["resources_list"] = json::value::array_t();
		result["reward"]["stackable_list"] = json::value::array_t();
		result["reward"]["text_id"] = 0;

        return result;
	}
}
