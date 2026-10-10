#include <std_include.hpp>

#include "cmd_boost_add_confirm.hpp"

#include "database/models/boosts.hpp"

namespace emulator::ssd
{
	json::value cmd_boost_add_confirm::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		param_t param{};
		if (!json::read(param, data))
		{
			return error(ERR_INVALIDARG);
		}

		const auto boost = database::boosts::get_boost_of_type(user->get_user_id(), param.type);
		if (!boost.has_value())
		{
			result["enable"] = 1;
		}
		else
		{
			result["enable"] = boost->get_time_left() + param.hour * 1h < database::boosts::max_boost_duration;
		}

		return result;
	}

	std::uint32_t cmd_boost_add_confirm::flags()
	{
		return CMD_NEEDS_USER;
	}
}
