#include <std_include.hpp>

#include "cmd_boost_check_active.hpp"

#include "database/models/boosts.hpp"

namespace emulator::ssd
{
	json::value cmd_boost_check_active::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		param_t param{};
		if (!json::read(param, data))
		{
			return error(ERR_INVALIDARG);
		}

		std::uint8_t flag{};
		
		union
		{
			std::uint32_t packed;
			std::uint8_t value[4];
		} rate{};

		rate.value[0] = 1;
		rate.value[1] = 1;
		rate.value[2] = 1;
		rate.value[3] = 1;

		const auto boost_list = database::boosts::get_boost_list(user->get_user_id());
		for (auto i = 0ull; i < boost_list.size(); i++)
		{
			flag |= (1 << boost_list[i].boost_type);

			switch (boost_list[i].boost_type)
			{
			case database::boosts::boost_type_resource:
				rate.value[0] = static_cast<std::uint8_t>(boost_list[i].mag1 / 100);
				rate.value[1] = static_cast<std::uint8_t>(boost_list[i].mag2 / 100);
				break;
			case database::boosts::boost_type_kub:
				rate.value[2] = static_cast<std::uint8_t>(boost_list[i].mag1 / 100);
				break;
			case database::boosts::boost_type_bp:
				rate.value[3] = static_cast<std::uint8_t>(boost_list[i].mag1 / 100);
				break;
			case database::boosts::boost_type_pass:
				break;
			}
		}

		if (flag != param.flag || rate.packed != param.rate)
		{
			return error(ERR_INACTIVE);
		}

		return result;
	}

	std::uint32_t cmd_boost_check_active::flags()
	{
		return CMD_NEEDS_USER;
	}
}
